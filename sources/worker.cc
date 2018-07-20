//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "worker.h"
#include "messages.h"
#include "plugin_processor.h"
#include <cassert>

#if 0
#   define trace(fmt, ...)
#else
#   define trace(fmt, ...) fprintf(stderr, "[Worker] " fmt "\n", ##__VA_ARGS__)
#endif

Worker::Worker(AdlplugAudioProcessor &proc)
    : Thread("Worker"),
      proc_(proc)
{
}

Worker::~Worker()
{
}

void Worker::stopWorker()
{
    if (this->isThreadRunning()) {
        signalThreadShouldExit();
        sem_.post();
        waitForThreadToExit(-1);
    }
}

void Worker::run()
{
    AdlplugAudioProcessor &proc = proc_;
    Semaphore &sem = sem_;

    Simple_Fifo &mq_recv = proc.message_queue_to_worker();
    Simple_Fifo &mq_send = proc.message_queue_for_worker();

    trace("Start");

    for (;;) {
        sem.wait();
        if (threadShouldExit())
            break;

#pragma message("TODO worker")

        while (Buffered_Message msg = read_message(mq_recv)) {
            switch (msg.header->tag) {
                default:
                    assert(false);
            }
            finish_read_message(mq_recv, msg);
        }
    }

    trace("Stop");
}
