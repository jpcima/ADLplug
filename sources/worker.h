//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "utility/semaphore.h"
class AdlplugAudioProcessor;

class Worker : public Thread {
public:
    explicit Worker(AdlplugAudioProcessor &proc);
    ~Worker();

    void stopWorker();
    void run();

    void postSemaphore()
        { sem_.post(); }

private:
    AdlplugAudioProcessor &proc_;
    Semaphore sem_;
};
