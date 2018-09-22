# ADLplug
Synthesizer plugin for ADLMIDI and OPNMIDI (VST/LV2)

![screenshot](docs/screen.png)

## Introduction

This software package provides FM synthesizer plugins, based on [OPL3](https://en.wikipedia.org/wiki/Yamaha_YMF262) and [OPN2](https://en.wikipedia.org/wiki/Yamaha_YM2612) sound chip emulations.  
The emulations and the drivers are provided by [libADLMIDI](https://github.com/Wohlstand/libADLMIDI) and [libOPNMIDI](https://github.com/Wohlstand/libOPNMIDI).

- [x] control of multiple YMF262/YM2612 emulated chips
- [x] high fidelity emulation, with choice of compromise level (good fidelity/fast, excellent fidelity/slow)
- [x] synthesis of melodic and percussive instruments
- [x] extensible polyphony
- [x] bundled collection of instruments
- [x] support for dynamic parameterization and automation
- [x] rigorous implementation of the MIDI standard
- [x] multi-channel operation with General MIDI compatibility
- [x] ability to synthesize entire MIDI files out of the box

Author: [Jean Pierre Cimalando](https://github.com/jpcima)  
Contributors: [Olivier Humbert](https://github.com/trebmuh), [Christopher Arndt](https://github.com/SpotlightKid)

## Development builds

[![Build Status](https://semaphoreci.com/api/v1/jpcima/adlplug-2/branches/master/badge.svg)](https://semaphoreci.com/jpcima/adlplug-2)

Find automatic builds of the development branch [here](http://jpcima.sdf1.org/software/development/ADLplug/).

## Useful links

- User Manual : [French :fr:](http://jpcima.sdf1.org/software/documentation/ADLplug/manual/fr/manual.html)
- LibraZiK-2 : [ADLplug :fr:](http://librazik.tuxfamily.org/doc2/logiciels/adlplug)
- Arch Linux AUR : [adlplug-git](https://aur.archlinux.org/packages/adlplug-git/) and [opnplug-git](https://aur.archlinux.org/packages/opnplug-git/)
- Bank editor software : [OPL3](https://github.com/Wohlstand/OPL3BankEditor) and [OPN2](https://github.com/Wohlstand/OPN2BankEditor)

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
cmake -DCMAKE_BUILD_TYPE=Release ..  #<more build options...>
cmake --build .
```

This package is able to build several plugins from a single source:
- to build the OPL3 variant, define the option `ADLplug_CHIP` to `OPL3`;
- to build the OPN2 variant, define the option `ADLplug_CHIP` to `OPN2`.

| Build option                                  | Description                                                     |
| --------------------------------------------- | --------------------------------------------------------------- |
| -DADLplug_VST2=ON/OFF                         | Build a VST2 plugin                                             |
| -DADLplug_VST3=ON/OFF                         | Build a VST3 plugin                                             |
| -DADLplug_LV2=ON/OFF                          | Build a LV2 plugin                                              |
| -DADLplug_Standalone=ON/OFF                   | Build a standalone program                                      |
| -DADLplug_Jack=ON/OFF                         | Build a standalone program for Jack with better features        |
| -DADLplug_CHIP=OPL3/OPN2                      | Build a variant for the given chip type (default: OPL3)         |
| -DADLplug_PCH=ON/OFF                          | Use precompiled headers, on a compiler which supports it        |
| -DADLplug_ASSERTIONS=ON/OFF                   | Force building with assertions regardless of build type         |

### Installing

```
sudo cmake --build . --target install
```

### Change Log

**1.0.0.beta.2 (dev)**

- added the freedesktop shortcuts and icons
- support for keyboard mappings other than QWERTY
- support setting the keyboard's octave
- highlighted the keys played via MIDI input
- made the program selection follow MIDI program change events
- allowed to install into the GNU standard installation directories
- we have been selected for the [Open Source Music FM Synthesizer Challenge](https://fmchallenge.osamc.de/fmsynths/)! :tada:

**1.0.0.beta.1**

- support of OPN2 synthesis in a distinct plugin
- fixed the plugin state which would be saved incomplete
- fixed the extension of OPN2 bank files in the file chooser

**1.0.0.alpha.3**

- compensation of MIDI latency at high buffer sizes
- fixed a mismanagement of the 4-op channel map
- fixed cases of bad channel allocations following a long idle period
- improved internal timing precision
- gained an ability to save and restore the current state
- added a large collection of embedded banks
- enhanced the UI in various ways
