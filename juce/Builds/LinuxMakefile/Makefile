# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# build with "V=1" for verbose builds
ifeq ($(V), 1)
V_AT =
else
V_AT = @
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

JUCE_ARCH_LABEL := $(shell uname -m)

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DADLMIDI_UNSTABLE_API -DADLMIDI_DISABLE_CPP_EXTRAS -DADLMIDI_DISABLE_MIDI_SEQUENCER -DADLMIDI_DISABLE_MUS_SUPPORT -DADLMIDI_DISABLE_XMI_SUPPORT -DVF_USE_FREETYPE=1 -DVF_USE_NATIVE_FREETYPE=1 -DJUCER_LINUX_MAKE_6A0FEE96=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(shell pkg-config --cflags freetype2 alsa x11 xext xinerama) -pthread -I$(HOME)/Documents/SDKs/VST_SDK/VST3_SDK -I../../../JuceLibraryCode -I../../../thirdparty/JUCE/modules -I../../../thirdparty/VFLib/modules -I../../../sources -I../../../thirdparty/libADLMIDI/include $(CPPFLAGS)

  JUCE_CPPFLAGS_VST := -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_Unity=0
  JUCE_CFLAGS_VST := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST := -shared -Wl,--no-undefined
  JUCE_TARGET_VST := ADLplug.so

  JUCE_CPPFLAGS_STANDALONE_PLUGIN := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0
  JUCE_TARGET_STANDALONE_PLUGIN := ADLplug

  JUCE_CPPFLAGS_SHARED_CODE := -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0 -DJUCE_SHARED_CODE=1
  JUCE_TARGET_SHARED_CODE := ADLplug.a

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -fPIC -g -ggdb -O0 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) $(shell pkg-config --libs freetype2 alsa x11 xext xinerama) -ldl -lpthread -lrt $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DADLMIDI_UNSTABLE_API -DADLMIDI_DISABLE_CPP_EXTRAS -DADLMIDI_DISABLE_MIDI_SEQUENCER -DADLMIDI_DISABLE_MUS_SUPPORT -DADLMIDI_DISABLE_XMI_SUPPORT -DVF_USE_FREETYPE=1 -DVF_USE_NATIVE_FREETYPE=1 -DJUCER_LINUX_MAKE_6A0FEE96=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(shell pkg-config --cflags freetype2 alsa x11 xext xinerama) -pthread -I$(HOME)/Documents/SDKs/VST_SDK/VST3_SDK -I../../../JuceLibraryCode -I../../../thirdparty/JUCE/modules -I../../../thirdparty/VFLib/modules -I../../../sources -I../../../thirdparty/libADLMIDI/include $(CPPFLAGS)

  JUCE_CPPFLAGS_VST := -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_Unity=0
  JUCE_CFLAGS_VST := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST := -shared -Wl,--no-undefined
  JUCE_TARGET_VST := ADLplug.so

  JUCE_CPPFLAGS_STANDALONE_PLUGIN := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0
  JUCE_TARGET_STANDALONE_PLUGIN := ADLplug

  JUCE_CPPFLAGS_SHARED_CODE := -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0 -DJUCE_SHARED_CODE=1
  JUCE_TARGET_SHARED_CODE := ADLplug.a

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -fPIC -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) $(shell pkg-config --libs freetype2 alsa x11 xext xinerama) -fvisibility=hidden -ldl -lpthread -lrt $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_ALL := \

OBJECTS_VST := \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST2_1e16d9b9.o \

OBJECTS_STANDALONE_PLUGIN := \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_Standalone_c6ff8483.o \

OBJECTS_SHARED_CODE := \
  $(JUCE_OBJDIR)/midi_dde9aea4.o \
  $(JUCE_OBJDIR)/rt_checker_e98c166d.o \
  $(JUCE_OBJDIR)/simple_fifo_14f04efe.o \
  $(JUCE_OBJDIR)/opl3_waves_d1747c0e.o \
  $(JUCE_OBJDIR)/about_component_574ed838.o \
  $(JUCE_OBJDIR)/graphics_utils_35f53d7c.o \
  $(JUCE_OBJDIR)/image_utils_a38eb576.o \
  $(JUCE_OBJDIR)/indicator_NxM_d416d5d0.o \
  $(JUCE_OBJDIR)/knob_component_5796d5a5.o \
  $(JUCE_OBJDIR)/knobman_skin_53c856a1.o \
  $(JUCE_OBJDIR)/look_and_feel_f9ca8b5.o \
  $(JUCE_OBJDIR)/main_component_ffc34342.o \
  $(JUCE_OBJDIR)/operator_editor_6b1eb07b.o \
  $(JUCE_OBJDIR)/styled_knobs_6e1b4b68.o \
  $(JUCE_OBJDIR)/vu_meter_b39aee30.o \
  $(JUCE_OBJDIR)/instrument_a889d85b.o \
  $(JUCE_OBJDIR)/player_38a5f121.o \
  $(JUCE_OBJDIR)/bank_manager_bcf4961e.o \
  $(JUCE_OBJDIR)/plugin_editor_4e133bb.o \
  $(JUCE_OBJDIR)/plugin_processor_d1dd99c2.o \
  $(JUCE_OBJDIR)/messages_ad8a073c.o \
  $(JUCE_OBJDIR)/wopl_file_21aeabfb.o \
  $(JUCE_OBJDIR)/dbopl_29fab7f2.o \
  $(JUCE_OBJDIR)/nukedopl3_7d9c6478.o \
  $(JUCE_OBJDIR)/nukedopl3_174_285c1147.o \
  $(JUCE_OBJDIR)/dosbox_opl3_7372c17d.o \
  $(JUCE_OBJDIR)/nuked_opl3_21e19815.o \
  $(JUCE_OBJDIR)/nuked_opl3_v174_1b09214c.o \
  $(JUCE_OBJDIR)/adldata_9238d314.o \
  $(JUCE_OBJDIR)/adlmidi_c578730b.o \
  $(JUCE_OBJDIR)/adlmidi_load_f4b93b64.o \
  $(JUCE_OBJDIR)/adlmidi_midiplay_ccf1053.o \
  $(JUCE_OBJDIR)/adlmidi_mus2mid_c51d72d.o \
  $(JUCE_OBJDIR)/adlmidi_opl3_7155cc66.o \
  $(JUCE_OBJDIR)/adlmidi_private_8636c74f.o \
  $(JUCE_OBJDIR)/adlmidi_xmi2mid_1b3ae4b6.o \
  $(JUCE_OBJDIR)/BinaryData_51699c3.o \
  $(JUCE_OBJDIR)/include_juce_audio_basics_68f957b.o \
  $(JUCE_OBJDIR)/include_juce_audio_devices_6eefc5f1.o \
  $(JUCE_OBJDIR)/include_juce_audio_formats_21d6c8f0.o \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_LV2_e2b46ad4.o \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_utils_baa4955d.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_5ced3317.o \
  $(JUCE_OBJDIR)/include_juce_audio_utils_a3a1cb85.o \
  $(JUCE_OBJDIR)/include_juce_core_d832d60c.o \
  $(JUCE_OBJDIR)/include_juce_data_structures_349db12.o \
  $(JUCE_OBJDIR)/include_juce_events_9b26cc86.o \
  $(JUCE_OBJDIR)/include_juce_graphics_eb811ef8.o \
  $(JUCE_OBJDIR)/include_juce_gui_basics_a2082cf6.o \
  $(JUCE_OBJDIR)/include_juce_gui_extra_e7ac9489.o \
  $(JUCE_OBJDIR)/include_vf_freetype_f81ee730.o \
  $(JUCE_OBJDIR)/include_vf_gui_2660237b.o \

.PHONY: clean all VST Standalone

all : VST Standalone

VST : $(JUCE_OUTDIR)/$(JUCE_TARGET_VST)
Standalone : $(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN)


$(JUCE_OUTDIR)/$(JUCE_TARGET_VST) : check-pkg-config $(OBJECTS_VST) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@echo Linking "ADLplug - VST"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_VST) $(OBJECTS_VST) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_VST) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN) : check-pkg-config $(OBJECTS_STANDALONE_PLUGIN) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@echo Linking "ADLplug - Standalone Plugin"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN) $(OBJECTS_STANDALONE_PLUGIN) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_STANDALONE_PLUGIN) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) : check-pkg-config $(OBJECTS_SHARED_CODE) $(RESOURCES)
	@echo Linking "ADLplug - Shared Code"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(AR) -rcs $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(OBJECTS_SHARED_CODE)

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST2_1e16d9b9.o: ../../../JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_VST2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_VST) $(JUCE_CFLAGS_VST) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_Standalone_c6ff8483.o: ../../../JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_Standalone.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_STANDALONE_PLUGIN) $(JUCE_CFLAGS_STANDALONE_PLUGIN) -o "$@" -c "$<"

$(JUCE_OBJDIR)/midi_dde9aea4.o: ../../../sources/utility/midi.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling midi.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/rt_checker_e98c166d.o: ../../../sources/utility/rt_checker.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling rt_checker.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/simple_fifo_14f04efe.o: ../../../sources/utility/simple_fifo.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling simple_fifo.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/opl3_waves_d1747c0e.o: ../../../sources/ui/chips/opl3_waves.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling opl3_waves.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/about_component_574ed838.o: ../../../sources/ui/about_component.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling about_component.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/graphics_utils_35f53d7c.o: ../../../sources/ui/graphics_utils.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling graphics_utils.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/image_utils_a38eb576.o: ../../../sources/ui/image_utils.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling image_utils.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/indicator_NxM_d416d5d0.o: ../../../sources/ui/indicator_NxM.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling indicator_NxM.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/knob_component_5796d5a5.o: ../../../sources/ui/knob_component.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling knob_component.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/knobman_skin_53c856a1.o: ../../../sources/ui/knobman_skin.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling knobman_skin.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/look_and_feel_f9ca8b5.o: ../../../sources/ui/look_and_feel.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling look_and_feel.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/main_component_ffc34342.o: ../../../sources/ui/main_component.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling main_component.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/operator_editor_6b1eb07b.o: ../../../sources/ui/operator_editor.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling operator_editor.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/styled_knobs_6e1b4b68.o: ../../../sources/ui/styled_knobs.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling styled_knobs.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/vu_meter_b39aee30.o: ../../../sources/ui/vu_meter.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling vu_meter.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/instrument_a889d85b.o: ../../../sources/adl/instrument.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling instrument.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/player_38a5f121.o: ../../../sources/adl/player.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling player.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/bank_manager_bcf4961e.o: ../../../sources/bank_manager.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling bank_manager.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/plugin_editor_4e133bb.o: ../../../sources/plugin_editor.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling plugin_editor.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/plugin_processor_d1dd99c2.o: ../../../sources/plugin_processor.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling plugin_processor.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/messages_ad8a073c.o: ../../../sources/messages.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling messages.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/wopl_file_21aeabfb.o: ../../../thirdparty/wopl/wopl/wopl_file.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling wopl_file.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/dbopl_29fab7f2.o: ../../../thirdparty/libADLMIDI/src/chips/dosbox/dbopl.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling dbopl.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/nukedopl3_7d9c6478.o: ../../../thirdparty/libADLMIDI/src/chips/nuked/nukedopl3.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling nukedopl3.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/nukedopl3_174_285c1147.o: ../../../thirdparty/libADLMIDI/src/chips/nuked/nukedopl3_174.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling nukedopl3_174.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/dosbox_opl3_7372c17d.o: ../../../thirdparty/libADLMIDI/src/chips/dosbox_opl3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling dosbox_opl3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/nuked_opl3_21e19815.o: ../../../thirdparty/libADLMIDI/src/chips/nuked_opl3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling nuked_opl3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/nuked_opl3_v174_1b09214c.o: ../../../thirdparty/libADLMIDI/src/chips/nuked_opl3_v174.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling nuked_opl3_v174.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adldata_9238d314.o: ../../../thirdparty/libADLMIDI/src/adldata.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adldata.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adlmidi_c578730b.o: ../../../thirdparty/libADLMIDI/src/adlmidi.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adlmidi.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adlmidi_load_f4b93b64.o: ../../../thirdparty/libADLMIDI/src/adlmidi_load.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adlmidi_load.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adlmidi_midiplay_ccf1053.o: ../../../thirdparty/libADLMIDI/src/adlmidi_midiplay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adlmidi_midiplay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adlmidi_mus2mid_c51d72d.o: ../../../thirdparty/libADLMIDI/src/adlmidi_mus2mid.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adlmidi_mus2mid.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adlmidi_opl3_7155cc66.o: ../../../thirdparty/libADLMIDI/src/adlmidi_opl3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adlmidi_opl3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adlmidi_private_8636c74f.o: ../../../thirdparty/libADLMIDI/src/adlmidi_private.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adlmidi_private.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adlmidi_xmi2mid_1b3ae4b6.o: ../../../thirdparty/libADLMIDI/src/adlmidi_xmi2mid.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adlmidi_xmi2mid.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_51699c3.o: ../../../JuceLibraryCode/BinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_basics_68f957b.o: ../../../JuceLibraryCode/include_juce_audio_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_devices_6eefc5f1.o: ../../../JuceLibraryCode/include_juce_audio_devices.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_devices.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_formats_21d6c8f0.o: ../../../JuceLibraryCode/include_juce_audio_formats.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_formats.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_LV2_e2b46ad4.o: ../../../JuceLibraryCode/include_juce_audio_plugin_client_LV2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_LV2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_utils_baa4955d.o: ../../../JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_plugin_client_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_5ced3317.o: ../../../JuceLibraryCode/include_juce_audio_processors.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_processors.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_utils_a3a1cb85.o: ../../../JuceLibraryCode/include_juce_audio_utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_core_d832d60c.o: ../../../JuceLibraryCode/include_juce_core.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_data_structures_349db12.o: ../../../JuceLibraryCode/include_juce_data_structures.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_data_structures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_events_9b26cc86.o: ../../../JuceLibraryCode/include_juce_events.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_events.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics_eb811ef8.o: ../../../JuceLibraryCode/include_juce_graphics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_graphics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_basics_a2082cf6.o: ../../../JuceLibraryCode/include_juce_gui_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_extra_e7ac9489.o: ../../../JuceLibraryCode/include_juce_gui_extra.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_extra.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_vf_freetype_f81ee730.o: ../../../JuceLibraryCode/include_vf_freetype.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_vf_freetype.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_vf_gui_2660237b.o: ../../../JuceLibraryCode/include_vf_gui.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_vf_gui.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors freetype2 alsa x11 xext xinerama

clean:
	@echo Cleaning ADLplug
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping ADLplug
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_VST:%.o=%.d)
-include $(OBJECTS_STANDALONE_PLUGIN:%.o=%.d)
-include $(OBJECTS_SHARED_CODE:%.o=%.d)
