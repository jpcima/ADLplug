# ADLplug
Synthesizer plugin for ADLMIDI (VST/LV2)

![screenshot](docs/screen.png)

## Introduction

This is a FM synthesizer based on [OPL3](https://en.wikipedia.org/wiki/Yamaha_YMF262) sound chip emulation.  
The emulation and the driver are provided by [libADLMIDI](https://github.com/Wohlstand/libADLMIDI).

## Development builds

[![Build Status](https://semaphoreci.com/api/v1/jpcima/adlplug-2/branches/master/badge.svg)](https://semaphoreci.com/jpcima/adlplug-2)

Find automatic builds of the development branch [here](http://jpcima.sdf1.org/software/development/ADLplug/).

## Build instructions

Install required dependencies:
- the CMake build system
- a C++11 compiler
- development packages for Linux: Jack, ALSA, Freetype, X11

### Compiling

```
git clone --recursive https://github.com/jpcima/ADLplug.git
mkdir ADLplug/build
cd ADLplug/build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

### Installing

```
sudo cmake --build . --target install
```

### Change Log

**1.0.0.alpha.3 (dev)**

- fixed a mismanagement of the 4-op channel map
- fixed cases of bad channel allocations following a long idle period
- improved internal timing precision
- gained an ability to save and restore the current state
- added a large collection of embedded banks
- enhanced the UI in various ways
