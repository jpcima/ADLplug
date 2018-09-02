//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"
#include "messages.h"
#include "adl/instrument.h"
#include "utility/semaphore.h"
#include <thread>
#include <atomic>
#include <unordered_map>
class AdlplugAudioProcessor;
class Simple_Fifo;
struct Buffered_Message;

class Worker {
public:
    explicit Worker(AdlplugAudioProcessor &proc);
    ~Worker();

    void start_worker();
    void stop_worker();

    void postSemaphore()
        { sem_.post(); }

private:
    void run();

private:
    AdlplugAudioProcessor &proc_;
    std::thread thread_;
    std::atomic<int> quit_;
    Semaphore sem_;
    std::unordered_map<uint32_t, Instrument> measure_requests_;

    void dispatch_messages();
    void handle_message(Buffered_Message &msg);
    void measure(uint32_t full_id, const Instrument &ins, Messages::Worker::MeasurementResult &body);
};
