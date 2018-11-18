//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "worker.h"
#include "plugin_processor.h"
#include "parameter_block.h"
#include "adl/measurer.h"
#include <chrono>
#include <cassert>
namespace stc = std::chrono;

#if 1
#   define trace(fmt, ...)
#else
#   define trace(fmt, ...) fprintf(stderr, "[Worker] " fmt "\n", ##__VA_ARGS__)
#endif

Worker::Worker(AdlplugAudioProcessor &proc)
    : proc_(proc)
{
}

Worker::~Worker()
{
    std::thread &th = thread_;
    if (th.joinable())
        th.join();
}

void Worker::start_worker()
{
    std::thread &th = thread_;
    stop_worker();
    quit_.store(0);
    th = std::thread([this]() { run(); });
}

void Worker::stop_worker()
{
    std::thread &th = thread_;
    if (th.joinable()) {
        quit_.store(1);
        sem_.post();
        th.join();
    }
}

void Worker::run()
{
    AdlplugAudioProcessor &proc = proc_;
    Semaphore &sem = sem_;

    Simple_Fifo &mq_recv = proc.message_queue_to_worker();
    Simple_Fifo &mq_send = proc.message_queue_for_worker();

    trace("Start");

    bool should_exit = false;
    while (!should_exit) {
        sem.wait();
        should_exit = quit_.load();
        if (should_exit)
            break;

        Buffered_Message msg_recv = Messages::read(mq_recv);
        assert(msg_recv);
        handle_message(msg_recv);
        Messages::finish_read(mq_recv, msg_recv);
        while (sem.try_wait() && !(should_exit = quit_.load())) {
            msg_recv = Messages::read(mq_recv);
            assert(msg_recv);
            handle_message(msg_recv);
            Messages::finish_read(mq_recv, msg_recv);
        }

        if (should_exit)
            break;

        if (!measure_requests_.empty()) {
            Message_Header hdr(Worker_Message::MeasurementResult, sizeof(Messages::Worker::MeasurementResult));
            Buffered_Message msg_send;
            while (!should_exit && !(msg_send = Messages::write(mq_send, hdr))) {
                std::this_thread::sleep_for(stc::milliseconds(1));
                while (sem.try_wait() && !(should_exit = quit_.load())) {
                    msg_recv = Messages::read(mq_recv);
                    assert(msg_recv);
                    handle_message(msg_recv);
                    Messages::finish_read(mq_recv, msg_recv);
                }
            }

            if (should_exit)
                break;

            auto &body = *(Messages::Worker::MeasurementResult *)msg_send.data;
            auto it = measure_requests_.begin();
            measure(it->first, it->second, body);
            Messages::finish_write(mq_send, msg_send);
            measure_requests_.erase(it);
        }
    }

    trace("Stop");
}

void Worker::handle_message(Buffered_Message &msg)
{
    AdlplugAudioProcessor &proc = proc_;

    Fx_Message tag = (Fx_Message)msg.header->tag;
    switch (tag) {
    case Fx_Message::RequestMeasurement: {
        const auto &body = *(const Messages::Fx::RequestMeasurement *)msg.data;
        Bank_Id id = body.bank;
        unsigned program = body.program;
        trace("Measurement requested for %c%u:%u:%u",
              id.percussive ? 'P' : 'M', id.msb, id.lsb, program);
        uint32_t full_id = (id.to_integer() << 7) | program;
        measure_requests_[full_id] = body.instrument;
        break;
    }
    case Fx_Message::RequestChipSettings: {
        const auto &body = *(const Messages::Fx::RequestChipSettings *)msg.data;
        unsigned emulator = body.cs.emulator;
        unsigned nchip = std::min(body.cs.chip_count, 100u);
        trace("Chip settings requested");
        std::unique_lock<std::mutex> lock = proc.acquire_player_nonrt();
        proc.panic_nonrt();
        proc.set_chip_emulator_nonrt(emulator);
        proc.set_num_chips_nonrt(nchip);
#if defined(ADLPLUG_OPL3)
        unsigned n4op = std::min(body.cs.fourop_count, 6 * nchip);
        proc.set_num_4ops_nonrt(n4op);
#elif defined(ADLPLUG_OPN2)
        proc.set_chip_type_nonrt(body.cs.chip_type);
#endif
        proc.mark_chip_settings_for_notification();
        break;
    }
    default:
        assert(false);
    }
}

void Worker::measure(uint32_t full_id, const Instrument &ins, Messages::Worker::MeasurementResult &body)
{
    Bank_Id id = Bank_Id::from_integer(full_id >> 7);
    unsigned program = full_id & 127;

    trace("Measuring for %c%u:%u:%u",
          id.percussive ? 'P' : 'M', id.msb, id.lsb, program);

    Measurer::DurationInfo result;
    Measurer::ComputeDurations(ins, result);

    trace("Finished measuring %c%u:%u:%u: %lu ms on, %lu ms off",
          id.percussive ? 'P' : 'M', id.msb, id.lsb, program,
          result.ms_sound_kon, result.ms_sound_koff);

    body.bank = id;
    body.program = program;
    body.instrument = ins;
    body.ms_sound_kon = result.ms_sound_kon;
    body.ms_sound_koff = result.ms_sound_koff;
}
