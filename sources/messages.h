//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "adl/instrument.h"
#include "adl/chip_settings.h"
#include "utility/simple_fifo.h"
#include "utility/counting_bitset.h"
#include "definitions.h"
#include <thread>
#include <cstdint>

enum class User_Message;
enum class Fx_Message;
enum class Worker_Message;

struct Message_Header {
    unsigned tag {};
    unsigned size = 0;
    Message_Header(User_Message tag, unsigned size)
        : tag((unsigned)tag), size(size) {}
    Message_Header(Fx_Message tag, unsigned size)
        : tag((unsigned)tag), size(size) {}
    Message_Header(Worker_Message tag, unsigned size)
        : tag((unsigned)tag), size(size) {}
};

struct Buffered_Message {
    Message_Header *header = nullptr;
    uint8_t *data = nullptr;
    unsigned offset = 0;
    explicit operator bool() const
        { return data; }
};

namespace Messages {
    Buffered_Message read(Simple_Fifo &fifo) noexcept;
    void finish_read(Simple_Fifo &fifo, const Buffered_Message &msg) noexcept;

    Buffered_Message write(Simple_Fifo &fifo, const Message_Header &hdr) noexcept;
    void finish_write(Simple_Fifo &fifo, Buffered_Message &msg) noexcept;

    template <class R, class P>
    Buffered_Message write_retrying(
        Simple_Fifo &fifo, const Message_Header &hdr, std::chrono::duration<R, P> delay)
    {
        Buffered_Message msg;
        while (!(msg = write(fifo, hdr)))
            std::this_thread::sleep_for(delay);
        return msg;
    }
}  // namespace Messages

//------------------------------------------------------------------------------
enum class User_Message {
    Midi = 0x1000,  // midi event
    RequestBankSlots,  // requests the layout of banks and instruments
    RequestFullBankState,  // requests all the managed bank state
    RequestChipSettings,  // requests chip settings
    ClearBanks,  // deletes all the managed banks
    LoadGlobalParameters,  // edits global parameters
    LoadInstrument,  // edits an instrument
    CreateInstrument,  // creates an instrument
    DeleteInstrument,  // deletes an instrument
    RenameBank,  // set bank name
    RenameProgram,  // set program name
    SelectProgram,  // changes selected program number
#if defined(ADLPLUG_OPL3)
    SelectOptimal4Ops,  // sets the optimal 4op channel count
#endif
};

namespace Messages {
namespace User {

struct RequestBankSlots {
    static constexpr User_Message tag = User_Message::RequestBankSlots;
};

struct RequestFullBankState {
    static constexpr User_Message tag = User_Message::RequestFullBankState;
};

struct RequestChipSettings {
    static constexpr User_Message tag = User_Message::RequestChipSettings;
};

struct ClearBanks {
    static constexpr User_Message tag = User_Message::ClearBanks;
    bool notify_back = false;
};

struct LoadGlobalParameters {
    static constexpr User_Message tag = User_Message::LoadGlobalParameters;
    Instrument_Global_Parameters param;
    bool notify_back = false;
};

struct LoadInstrument {
    static constexpr User_Message tag = User_Message::LoadInstrument;
    unsigned part = 0;
    Bank_Id bank;
    uint8_t program = 0;
    Instrument instrument;
    bool need_measurement = false;
    bool notify_back = false;
};

struct CreateInstrument {
    static constexpr User_Message tag = User_Message::CreateInstrument;
    Bank_Id bank;
    uint8_t program = 0;
    bool notify_back = false;
};

struct DeleteInstrument {
    static constexpr User_Message tag = User_Message::DeleteInstrument;
    Bank_Id bank;
    uint8_t program = 0;
    bool notify_back = false;
};

struct RenameBank {
    static constexpr User_Message tag = User_Message::RenameBank;
    Bank_Id bank;
    bool notify_back = false;
    char name[32] {};
};

struct RenameProgram {
    static constexpr User_Message tag = User_Message::RenameProgram;
    Bank_Id bank;
    uint8_t program = 0;
    bool notify_back = false;
    char name[32] {};
};

struct SelectProgram {
    static constexpr User_Message tag = User_Message::SelectProgram;
    unsigned part = 0;
    Bank_Id bank;
    uint8_t program = 0;
};

#if defined(ADLPLUG_OPL3)
struct SelectOptimal4Ops {
    static constexpr User_Message tag = User_Message::SelectOptimal4Ops;
};
#endif

}  // namespace User
}  // namespace Messages

//------------------------------------------------------------------------------
enum class Fx_Message {
    NotifyReady = 0x2000,  // notifies readiness
    NotifyBankSlots,  // notifies the layout of banks and instruments
    NotifyGlobalParameters,  // notifies the global parameters
    NotifyInstrument,  // notifies an instrument when changed
    NotifyChipSettings,  // notifies chip settings when changed
    NotifySelection,  // notifies selection when changed
    RequestMeasurement,  // request measurement of a program
    RequestChipSettings,  // request a change of chip settings
};

namespace Messages {
namespace Fx {

struct NotifyReady {
    static constexpr Fx_Message tag = Fx_Message::NotifyReady;
};

struct NotifyBankSlots {
    static constexpr Fx_Message tag = Fx_Message::NotifyBankSlots;
    struct Entry {
        Bank_Id bank;
        counting_bitset<128> used;
        char name[32] {};
    };
    unsigned count;
    Entry entry[bank_reserve_size];
};

struct NotifyGlobalParameters {
    static constexpr Fx_Message tag = Fx_Message::NotifyGlobalParameters;
    Instrument_Global_Parameters param;
};

struct NotifyInstrument {
    static constexpr Fx_Message tag = Fx_Message::NotifyInstrument;
    Bank_Id bank;
    uint8_t program = 0;
    Instrument instrument;
};

struct NotifyChipSettings {
    static constexpr Fx_Message tag = Fx_Message::NotifyChipSettings;
    Chip_Settings cs;
};

struct NotifySelection {
    static constexpr Fx_Message tag = Fx_Message::NotifySelection;
    unsigned part = 0;
    Bank_Id bank;
    uint8_t program = 0;
};

struct RequestMeasurement {
    static constexpr Fx_Message tag = Fx_Message::RequestMeasurement;
    Bank_Id bank;
    uint8_t program = 0;
    Instrument instrument;
};

struct RequestChipSettings {
    static constexpr Fx_Message tag = Fx_Message::RequestChipSettings;
    Chip_Settings cs;
};

}  // namespace Fx
}  // namespace Messages

//------------------------------------------------------------------------------
enum class Worker_Message {
    MeasurementResult = 0x3000,  // result of a measurement operation
};

namespace Messages {
namespace Worker {

struct MeasurementResult {
    static constexpr Worker_Message tag = Worker_Message::MeasurementResult;
    Bank_Id bank;
    uint8_t program = 0;
    Instrument instrument;
    uint16_t ms_sound_kon = 0;
    uint16_t ms_sound_koff = 0;
};

}  // namespace Worker
}  // namespace Messages
