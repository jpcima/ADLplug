//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#if defined(ADLPLUG_OPL3)
#include <wopl/wopl_file.h>
#elif defined(ADLPLUG_OPN2)
#include <wopn/wopn_file.h>
#endif

struct WOPx {

#if defined(ADLPLUG_OPL3)

#define WOPx_BANK_FORMAT "WOPL"
#define WOPx_BANK_SUFFIX "wopl"
#define WOPx_INST_FORMAT "OPLI"
#define WOPx_INST_SUFFIX "opli"

enum {
    FileVersion = 3,
};

enum {
    Ins_IsBlank = WOPL_Ins_IsBlank,
};

typedef WOPLInstrument Instrument;
typedef WOPLBank Bank;
typedef WOPIFile InstrumentFile;
typedef WOPLFile BankFile;

static constexpr auto &LoadBankFromMem = WOPL_LoadBankFromMem;
static constexpr auto &LoadInstFromMem = WOPL_LoadInstFromMem;
static constexpr auto &CalculateBankFileSize = WOPL_CalculateBankFileSize;
static constexpr auto &CalculateInstFileSize = WOPL_CalculateInstFileSize;
static constexpr auto &SaveBankToMem = WOPL_SaveBankToMem;
static constexpr auto &SaveInstToMem = WOPL_SaveInstToMem;

typedef WOPLFile_Deleter BankFile_Deleter;
typedef WOPLFile_Ptr BankFile_Ptr;

#elif defined(ADLPLUG_OPN2)

#define WOPx_BANK_FORMAT "WOPN"
#define WOPx_BANK_SUFFIX "wopn"
#define WOPx_INST_FORMAT "OPNI"
#define WOPx_INST_SUFFIX "opni"

enum {
    FileVersion = 2,
};

enum {
    Ins_IsBlank = WOPN_Ins_IsBlank,
};

typedef WOPNInstrument Instrument;
typedef WOPNBank Bank;
typedef OPNIFile InstrumentFile;
typedef WOPNFile BankFile;

static constexpr auto &LoadBankFromMem = WOPN_LoadBankFromMem;
static constexpr auto &LoadInstFromMem = WOPN_LoadInstFromMem;
static constexpr auto &CalculateBankFileSize = WOPN_CalculateBankFileSize;
static constexpr auto &CalculateInstFileSize = WOPN_CalculateInstFileSize;
static constexpr auto &SaveBankToMem = WOPN_SaveBankToMem;
static constexpr auto &SaveInstToMem = WOPN_SaveInstToMem;

typedef WOPNFile_Deleter BankFile_Deleter;
typedef WOPNFile_Ptr BankFile_Ptr;

#endif

};
