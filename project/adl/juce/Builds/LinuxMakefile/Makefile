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

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DADLPLUG_OPL3 -DADLMIDI_UNSTABLE_API -DADLMIDI_DISABLE_CPP_EXTRAS -DADLMIDI_DISABLE_MIDI_SEQUENCER -DADLMIDI_DISABLE_MUS_SUPPORT -DADLMIDI_DISABLE_XMI_SUPPORT -DJUCER_LINUX_MAKE_6A0FEE96=1 -DJUCE_APP_VERSION=1.0.2 -DJUCE_APP_VERSION_HEX=0x10002 $(shell pkg-config --cflags freetype2 alsa x11 xext xinerama) -pthread -I../../../../../thirdparty/JUCE/modules/juce_audio_processors/format_types/VST3_SDK -I../../../JuceLibraryCode -I../../../../../thirdparty/JUCE/modules -I../../../../../sources -I../../../../../thirdparty/libADLMIDI/include -I../../../../../thirdparty/wopl -I../../../../../thirdparty/simpleini -I../../../../../thirdparty/fmt/include -I../../../../../thirdparty/folly $(CPPFLAGS)

  JUCE_CPPFLAGS_VST := -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_Unity=0
  JUCE_CFLAGS_VST := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST := -shared -Wl,--no-undefined
  JUCE_TARGET_VST := ADLplug.so

  JUCE_CPPFLAGS_STANDALONE_PLUGIN := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0
  JUCE_TARGET_STANDALONE_PLUGIN := ADLplug

  JUCE_CPPFLAGS_SHARED_CODE := -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0 -DJUCE_SHARED_CODE=1
  JUCE_TARGET_SHARED_CODE := ADLplug.a

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -fPIC -g -ggdb -O0 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++11 $(CXXFLAGS)
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

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DADLPLUG_OPL3 -DADLMIDI_UNSTABLE_API -DADLMIDI_DISABLE_CPP_EXTRAS -DADLMIDI_DISABLE_MIDI_SEQUENCER -DADLMIDI_DISABLE_MUS_SUPPORT -DADLMIDI_DISABLE_XMI_SUPPORT -DJUCER_LINUX_MAKE_6A0FEE96=1 -DJUCE_APP_VERSION=1.0.2 -DJUCE_APP_VERSION_HEX=0x10002 $(shell pkg-config --cflags freetype2 alsa x11 xext xinerama) -pthread -I../../../../../thirdparty/JUCE/modules/juce_audio_processors/format_types/VST3_SDK -I../../../JuceLibraryCode -I../../../../../thirdparty/JUCE/modules -I../../../../../sources -I../../../../../thirdparty/libADLMIDI/include -I../../../../../thirdparty/wopl -I../../../../../thirdparty/simpleini -I../../../../../thirdparty/fmt/include -I../../../../../thirdparty/folly $(CPPFLAGS)

  JUCE_CPPFLAGS_VST := -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_Unity=0
  JUCE_CFLAGS_VST := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST := -shared -Wl,--no-undefined
  JUCE_TARGET_VST := ADLplug.so

  JUCE_CPPFLAGS_STANDALONE_PLUGIN := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0
  JUCE_TARGET_STANDALONE_PLUGIN := ADLplug

  JUCE_CPPFLAGS_SHARED_CODE := -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0 -DJUCE_SHARED_CODE=1
  JUCE_TARGET_SHARED_CODE := ADLplug.a

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -fPIC -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++11 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) $(shell pkg-config --libs freetype2 alsa x11 xext xinerama) -fvisibility=hidden -ldl -lpthread -lrt $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_ALL := \

OBJECTS_VST := \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_VST2_1e16d9b9.o \

OBJECTS_STANDALONE_PLUGIN := \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_Standalone_c6ff8483.o \

OBJECTS_SHARED_CODE := \
  $(JUCE_OBJDIR)/bank_manager_aad34bbe.o \
  $(JUCE_OBJDIR)/configuration_bf71fffe.o \
  $(JUCE_OBJDIR)/messages_bd648cdc.o \
  $(JUCE_OBJDIR)/plugin_editor_d2d9321b.o \
  $(JUCE_OBJDIR)/plugin_processor_a0287f62.o \
  $(JUCE_OBJDIR)/plugin_state_37e59603.o \
  $(JUCE_OBJDIR)/resources_23ff376f.o \
  $(JUCE_OBJDIR)/worker_5b6a13ea.o \
  $(JUCE_OBJDIR)/insnames_1e4d446c.o \
  $(JUCE_OBJDIR)/chip_settings_ba52771f.o \
  $(JUCE_OBJDIR)/instrument_d2606ca0.o \
  $(JUCE_OBJDIR)/instrument_properties_5fc3ca2a.o \
  $(JUCE_OBJDIR)/player_26936fe6.o \
  $(JUCE_OBJDIR)/measurer_f1b2d6bd.o \
  $(JUCE_OBJDIR)/dosbox_opl3_82e22dbd.o \
  $(JUCE_OBJDIR)/dbopl_878d3b2.o \
  $(JUCE_OBJDIR)/algorithm_help_d8b076be.o \
  $(JUCE_OBJDIR)/opl3_waves_901efa8c.o \
  $(JUCE_OBJDIR)/about_component_cf87c1fd.o \
  $(JUCE_OBJDIR)/main_component_c1938cdd.o \
  $(JUCE_OBJDIR)/operator_editor_e3579a40.o \
  $(JUCE_OBJDIR)/parameter_block_94463d58.o \
  $(JUCE_OBJDIR)/look_and_feel_dde78e55.o \
  $(JUCE_OBJDIR)/algorithm_component_4451cbc5.o \
  $(JUCE_OBJDIR)/indicator_NxM_67a37cbf.o \
  $(JUCE_OBJDIR)/knob_component_359f0c96.o \
  $(JUCE_OBJDIR)/midi_keyboard_ex_450d917d.o \
  $(JUCE_OBJDIR)/new_program_editor_24fa3823.o \
  $(JUCE_OBJDIR)/program_name_editor_82e089ec.o \
  $(JUCE_OBJDIR)/styled_knobs_d5f68a99.o \
  $(JUCE_OBJDIR)/vu_meter_6e0f6de1.o \
  $(JUCE_OBJDIR)/wave_label_964e0c9c.o \
  $(JUCE_OBJDIR)/graphics_140b15ab.o \
  $(JUCE_OBJDIR)/image_d1e553cb.o \
  $(JUCE_OBJDIR)/key_maps_fbe9243f.o \
  $(JUCE_OBJDIR)/knobman_skin_9f0d575e.o \
  $(JUCE_OBJDIR)/midi_cbc86444.o \
  $(JUCE_OBJDIR)/pak_2c4b415d.o \
  $(JUCE_OBJDIR)/rt_checker_50b4140d.o \
  $(JUCE_OBJDIR)/simple_fifo_92c8055e.o \
  $(JUCE_OBJDIR)/format_e61127c9.o \
  $(JUCE_OBJDIR)/posix_cd6b0359.o \
  $(JUCE_OBJDIR)/wopl_file_88724a5b.o \
  $(JUCE_OBJDIR)/adldata_2ed930b4.o \
  $(JUCE_OBJDIR)/adlmidi_6218d0ab.o \
  $(JUCE_OBJDIR)/adlmidi_load_ffd261c4.o \
  $(JUCE_OBJDIR)/adlmidi_midiplay_2e06b3.o \
  $(JUCE_OBJDIR)/adlmidi_opl3_7c6ef2c6.o \
  $(JUCE_OBJDIR)/adlmidi_private_9ef84ef.o \
  $(JUCE_OBJDIR)/java_opl3_d38ec45e.o \
  $(JUCE_OBJDIR)/opal_opl3_37ebc7f4.o \
  $(JUCE_OBJDIR)/dosbox_opl3_ebf2971d.o \
  $(JUCE_OBJDIR)/nuked_opl3_15408e75.o \
  $(JUCE_OBJDIR)/nuked_opl3_v174_fe6526ec.o \
  $(JUCE_OBJDIR)/dbopl_c1759652.o \
  $(JUCE_OBJDIR)/nukedopl3_d57d5218.o \
  $(JUCE_OBJDIR)/nukedopl3_174_a4cd2ee7.o \
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

.PHONY: clean all strip VST Standalone

all : VST Standalone

VST : $(JUCE_OUTDIR)/$(JUCE_TARGET_VST)
Standalone : $(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN)


$(JUCE_OUTDIR)/$(JUCE_TARGET_VST) : $(OBJECTS_VST) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors freetype2 alsa x11 xext xinerama
	@echo Linking "ADLplug - VST"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_VST) $(OBJECTS_VST) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_VST) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN) : $(OBJECTS_STANDALONE_PLUGIN) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors freetype2 alsa x11 xext xinerama
	@echo Linking "ADLplug - Standalone Plugin"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN) $(OBJECTS_STANDALONE_PLUGIN) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_STANDALONE_PLUGIN) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) : $(OBJECTS_SHARED_CODE) $(RESOURCES)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors freetype2 alsa x11 xext xinerama
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

$(JUCE_OBJDIR)/bank_manager_aad34bbe.o: ../../../../../sources/bank_manager.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling bank_manager.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/configuration_bf71fffe.o: ../../../../../sources/configuration.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling configuration.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/messages_bd648cdc.o: ../../../../../sources/messages.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling messages.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/plugin_editor_d2d9321b.o: ../../../../../sources/plugin_editor.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling plugin_editor.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/plugin_processor_a0287f62.o: ../../../../../sources/plugin_processor.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling plugin_processor.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/plugin_state_37e59603.o: ../../../../../sources/plugin_state.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling plugin_state.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/resources_23ff376f.o: ../../../../../sources/resources.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling resources.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/worker_5b6a13ea.o: ../../../../../sources/worker.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling worker.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/insnames_1e4d446c.o: ../../../../../sources/midi/insnames.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling insnames.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/chip_settings_ba52771f.o: ../../../../../sources/opl3/adl/chip_settings.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling chip_settings.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/instrument_d2606ca0.o: ../../../../../sources/opl3/adl/instrument.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling instrument.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/instrument_properties_5fc3ca2a.o: ../../../../../sources/opl3/adl/instrument_properties.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling instrument_properties.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/player_26936fe6.o: ../../../../../sources/opl3/adl/player.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling player.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/measurer_f1b2d6bd.o: ../../../../../sources/opl3/adl/measurer/measurer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling measurer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/dosbox_opl3_82e22dbd.o: ../../../../../sources/opl3/adl/measurer/chips/dosbox_opl3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling dosbox_opl3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/dbopl_878d3b2.o: ../../../../../sources/opl3/adl/measurer/chips/dosbox/dbopl.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling dbopl.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/algorithm_help_d8b076be.o: ../../../../../sources/opl3/ui/components/algorithm_help.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling algorithm_help.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/opl3_waves_901efa8c.o: ../../../../../sources/opl3/ui/components/opl3_waves.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling opl3_waves.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/about_component_cf87c1fd.o: ../../../../../sources/opl3/ui/about_component.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling about_component.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/main_component_c1938cdd.o: ../../../../../sources/opl3/ui/main_component.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling main_component.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/operator_editor_e3579a40.o: ../../../../../sources/opl3/ui/operator_editor.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling operator_editor.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/parameter_block_94463d58.o: ../../../../../sources/opl3/parameter_block.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling parameter_block.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/look_and_feel_dde78e55.o: ../../../../../sources/ui/look_and_feel.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling look_and_feel.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/algorithm_component_4451cbc5.o: ../../../../../sources/ui/components/algorithm_component.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling algorithm_component.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/indicator_NxM_67a37cbf.o: ../../../../../sources/ui/components/indicator_NxM.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling indicator_NxM.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/knob_component_359f0c96.o: ../../../../../sources/ui/components/knob_component.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling knob_component.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/midi_keyboard_ex_450d917d.o: ../../../../../sources/ui/components/midi_keyboard_ex.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling midi_keyboard_ex.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/new_program_editor_24fa3823.o: ../../../../../sources/ui/components/new_program_editor.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling new_program_editor.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/program_name_editor_82e089ec.o: ../../../../../sources/ui/components/program_name_editor.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling program_name_editor.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/styled_knobs_d5f68a99.o: ../../../../../sources/ui/components/styled_knobs.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling styled_knobs.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/vu_meter_6e0f6de1.o: ../../../../../sources/ui/components/vu_meter.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling vu_meter.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/wave_label_964e0c9c.o: ../../../../../sources/ui/components/wave_label.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling wave_label.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/graphics_140b15ab.o: ../../../../../sources/ui/utility/graphics.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling graphics.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/image_d1e553cb.o: ../../../../../sources/ui/utility/image.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling image.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/key_maps_fbe9243f.o: ../../../../../sources/ui/utility/key_maps.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling key_maps.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/knobman_skin_9f0d575e.o: ../../../../../sources/ui/utility/knobman_skin.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling knobman_skin.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/midi_cbc86444.o: ../../../../../sources/utility/midi.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling midi.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pak_2c4b415d.o: ../../../../../sources/utility/pak.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pak.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/rt_checker_50b4140d.o: ../../../../../sources/utility/rt_checker.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling rt_checker.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/simple_fifo_92c8055e.o: ../../../../../sources/utility/simple_fifo.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling simple_fifo.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/format_e61127c9.o: ../../../../../thirdparty/fmt/src/format.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling format.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/posix_cd6b0359.o: ../../../../../thirdparty/fmt/src/posix.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling posix.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/wopl_file_88724a5b.o: ../../../../../thirdparty/wopl/wopl/wopl_file.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling wopl_file.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adldata_2ed930b4.o: ../../../../../thirdparty/libADLMIDI/src/adldata.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adldata.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adlmidi_6218d0ab.o: ../../../../../thirdparty/libADLMIDI/src/adlmidi.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adlmidi.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adlmidi_load_ffd261c4.o: ../../../../../thirdparty/libADLMIDI/src/adlmidi_load.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adlmidi_load.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adlmidi_midiplay_2e06b3.o: ../../../../../thirdparty/libADLMIDI/src/adlmidi_midiplay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adlmidi_midiplay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adlmidi_opl3_7c6ef2c6.o: ../../../../../thirdparty/libADLMIDI/src/adlmidi_opl3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adlmidi_opl3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/adlmidi_private_9ef84ef.o: ../../../../../thirdparty/libADLMIDI/src/adlmidi_private.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling adlmidi_private.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/java_opl3_d38ec45e.o: ../../../../../thirdparty/libADLMIDI/src/chips/java_opl3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling java_opl3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/opal_opl3_37ebc7f4.o: ../../../../../thirdparty/libADLMIDI/src/chips/opal_opl3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling opal_opl3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/dosbox_opl3_ebf2971d.o: ../../../../../thirdparty/libADLMIDI/src/chips/dosbox_opl3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling dosbox_opl3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/nuked_opl3_15408e75.o: ../../../../../thirdparty/libADLMIDI/src/chips/nuked_opl3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling nuked_opl3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/nuked_opl3_v174_fe6526ec.o: ../../../../../thirdparty/libADLMIDI/src/chips/nuked_opl3_v174.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling nuked_opl3_v174.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/dbopl_c1759652.o: ../../../../../thirdparty/libADLMIDI/src/chips/dosbox/dbopl.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling dbopl.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/nukedopl3_d57d5218.o: ../../../../../thirdparty/libADLMIDI/src/chips/nuked/nukedopl3.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling nukedopl3.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/nukedopl3_174_a4cd2ee7.o: ../../../../../thirdparty/libADLMIDI/src/chips/nuked/nukedopl3_174.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling nukedopl3_174.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

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

clean:
	@echo Cleaning ADLplug
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping ADLplug
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_VST:%.o=%.d)
-include $(OBJECTS_STANDALONE_PLUGIN:%.o=%.d)
-include $(OBJECTS_SHARED_CODE:%.o=%.d)
