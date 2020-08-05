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

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DADLPLUG_OPN2 -DOPNMIDI_UNSTABLE_API -DOPNMIDI_DISABLE_GX_EMULATOR -DOPNMIDI_DISABLE_CPP_EXTRAS -DOPNMIDI_DISABLE_MIDI_SEQUENCER -DOPNMIDI_DISABLE_MUS_SUPPORT -DOPNMIDI_DISABLE_XMI_SUPPORT -DJUCER_LINUX_MAKE_6A0FEE96=1 -DJUCE_APP_VERSION=1.0.2 -DJUCE_APP_VERSION_HEX=0x10002 $(shell pkg-config --cflags freetype2 alsa x11 xext xinerama) -pthread -I../../../../../thirdparty/JUCE/modules/juce_audio_processors/format_types/VST3_SDK -I../../../JuceLibraryCode -I../../../../../thirdparty/JUCE/modules -I../../../../../sources -I../../../../../thirdparty/libOPNMIDI/include -I../../../../../thirdparty/wopn -I../../../../../thirdparty/simpleini -I../../../../../thirdparty/fmt/include -I../../../../../thirdparty/folly $(CPPFLAGS)

  JUCE_CPPFLAGS_VST := -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_Unity=0
  JUCE_CFLAGS_VST := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST := -shared -Wl,--no-undefined
  JUCE_TARGET_VST := OPNplug.so

  JUCE_CPPFLAGS_STANDALONE_PLUGIN := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0
  JUCE_TARGET_STANDALONE_PLUGIN := OPNplug

  JUCE_CPPFLAGS_SHARED_CODE := -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0 -DJUCE_SHARED_CODE=1
  JUCE_TARGET_SHARED_CODE := OPNplug.a

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

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DADLPLUG_OPN2 -DOPNMIDI_UNSTABLE_API -DOPNMIDI_DISABLE_GX_EMULATOR -DOPNMIDI_DISABLE_CPP_EXTRAS -DOPNMIDI_DISABLE_MIDI_SEQUENCER -DOPNMIDI_DISABLE_MUS_SUPPORT -DOPNMIDI_DISABLE_XMI_SUPPORT -DJUCER_LINUX_MAKE_6A0FEE96=1 -DJUCE_APP_VERSION=1.0.2 -DJUCE_APP_VERSION_HEX=0x10002 $(shell pkg-config --cflags freetype2 alsa x11 xext xinerama) -pthread -I../../../../../thirdparty/JUCE/modules/juce_audio_processors/format_types/VST3_SDK -I../../../JuceLibraryCode -I../../../../../thirdparty/JUCE/modules -I../../../../../sources -I../../../../../thirdparty/libOPNMIDI/include -I../../../../../thirdparty/wopn -I../../../../../thirdparty/simpleini -I../../../../../thirdparty/fmt/include -I../../../../../thirdparty/folly $(CPPFLAGS)

  JUCE_CPPFLAGS_VST := -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0 -DJucePlugin_Build_Unity=0
  JUCE_CFLAGS_VST := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_VST := -shared -Wl,--no-undefined
  JUCE_TARGET_VST := OPNplug.so

  JUCE_CPPFLAGS_STANDALONE_PLUGIN := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0
  JUCE_TARGET_STANDALONE_PLUGIN := OPNplug

  JUCE_CPPFLAGS_SHARED_CODE := -DJucePlugin_Build_VST=1 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=1 -DJucePlugin_Build_Unity=0 -DJUCE_SHARED_CODE=1
  JUCE_TARGET_SHARED_CODE := OPNplug.a

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
  $(JUCE_OBJDIR)/parameter_block_c5d651fb.o \
  $(JUCE_OBJDIR)/chip_settings_c82fef02.o \
  $(JUCE_OBJDIR)/instrument_d3f9b79d.o \
  $(JUCE_OBJDIR)/instrument_properties_88e8af0d.o \
  $(JUCE_OBJDIR)/player_8fdbc163.o \
  $(JUCE_OBJDIR)/measurer_9a03d4ba.o \
  $(JUCE_OBJDIR)/mame_opn2_15456754.o \
  $(JUCE_OBJDIR)/mame_ym2612fm_47977c5e.o \
  $(JUCE_OBJDIR)/about_component_7d59467a.o \
  $(JUCE_OBJDIR)/main_component_cf7104c0.o \
  $(JUCE_OBJDIR)/operator_editor_91291ebd.o \
  $(JUCE_OBJDIR)/algorithm_help_d51842fb.o \
  $(JUCE_OBJDIR)/ssgeg_waves_4ccbf4ec.o \
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
  $(JUCE_OBJDIR)/wopn_file_be5e7559.o \
  $(JUCE_OBJDIR)/opnmidi_98c1ceb3.o \
  $(JUCE_OBJDIR)/opnmidi_load_2ebee8bc.o \
  $(JUCE_OBJDIR)/opnmidi_midiplay_470e51ab.o \
  $(JUCE_OBJDIR)/opnmidi_opn2_aeb7137b.o \
  $(JUCE_OBJDIR)/opnmidi_private_a0ddfaf7.o \
  $(JUCE_OBJDIR)/Ym2612_Emu_dd631ef0.o \
  $(JUCE_OBJDIR)/mame_ym2612fm_bf0c8cbd.o \
  $(JUCE_OBJDIR)/emu2149_c4616ba2.o \
  $(JUCE_OBJDIR)/fm_7c41fee0.o \
  $(JUCE_OBJDIR)/resampler_28ddff46.o \
  $(JUCE_OBJDIR)/ymdeltat_6208b669.o \
  $(JUCE_OBJDIR)/fmgen_file_b2ae057e.o \
  $(JUCE_OBJDIR)/fmgen_fmgen_3d85cdb1.o \
  $(JUCE_OBJDIR)/fmgen_fmtimer_43f10826.o \
  $(JUCE_OBJDIR)/fmgen_opna_cf158b56.o \
  $(JUCE_OBJDIR)/fmgen_psg_71c63a8c.o \
  $(JUCE_OBJDIR)/ym3438_c6367e33.o \
  $(JUCE_OBJDIR)/gens_opn2_2f9f063e.o \
  $(JUCE_OBJDIR)/gx_opn2_c6d72e30.o \
  $(JUCE_OBJDIR)/mame_opn2_b701b555.o \
  $(JUCE_OBJDIR)/mame_opna_b9980604.o \
  $(JUCE_OBJDIR)/np2_opna_d8992a70.o \
  $(JUCE_OBJDIR)/nuked_opn2_878260ae.o \
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
	@echo Linking "OPNplug - VST"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_VST) $(OBJECTS_VST) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_VST) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN) : $(OBJECTS_STANDALONE_PLUGIN) $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors freetype2 alsa x11 xext xinerama
	@echo Linking "OPNplug - Standalone Plugin"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_STANDALONE_PLUGIN) $(OBJECTS_STANDALONE_PLUGIN) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_STANDALONE_PLUGIN) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) : $(OBJECTS_SHARED_CODE) $(RESOURCES)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors freetype2 alsa x11 xext xinerama
	@echo Linking "OPNplug - Shared Code"
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

$(JUCE_OBJDIR)/parameter_block_c5d651fb.o: ../../../../../sources/opn2/parameter_block.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling parameter_block.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/chip_settings_c82fef02.o: ../../../../../sources/opn2/adl/chip_settings.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling chip_settings.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/instrument_d3f9b79d.o: ../../../../../sources/opn2/adl/instrument.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling instrument.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/instrument_properties_88e8af0d.o: ../../../../../sources/opn2/adl/instrument_properties.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling instrument_properties.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/player_8fdbc163.o: ../../../../../sources/opn2/adl/player.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling player.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/measurer_9a03d4ba.o: ../../../../../sources/opn2/adl/measurer/measurer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling measurer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/mame_opn2_15456754.o: ../../../../../sources/opn2/adl/measurer/chips/mame_opn2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling mame_opn2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/mame_ym2612fm_47977c5e.o: ../../../../../sources/opn2/adl/measurer/chips/mame/mame_ym2612fm.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling mame_ym2612fm.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/about_component_7d59467a.o: ../../../../../sources/opn2/ui/about_component.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling about_component.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/main_component_cf7104c0.o: ../../../../../sources/opn2/ui/main_component.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling main_component.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/operator_editor_91291ebd.o: ../../../../../sources/opn2/ui/operator_editor.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling operator_editor.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/algorithm_help_d51842fb.o: ../../../../../sources/opn2/ui/components/algorithm_help.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling algorithm_help.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ssgeg_waves_4ccbf4ec.o: ../../../../../sources/opn2/ui/components/ssgeg_waves.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ssgeg_waves.cc"
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

$(JUCE_OBJDIR)/wopn_file_be5e7559.o: ../../../../../thirdparty/wopn/wopn/wopn_file.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling wopn_file.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/opnmidi_98c1ceb3.o: ../../../../../thirdparty/libOPNMIDI/src/opnmidi.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling opnmidi.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/opnmidi_load_2ebee8bc.o: ../../../../../thirdparty/libOPNMIDI/src/opnmidi_load.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling opnmidi_load.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/opnmidi_midiplay_470e51ab.o: ../../../../../thirdparty/libOPNMIDI/src/opnmidi_midiplay.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling opnmidi_midiplay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/opnmidi_opn2_aeb7137b.o: ../../../../../thirdparty/libOPNMIDI/src/opnmidi_opn2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling opnmidi_opn2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/opnmidi_private_a0ddfaf7.o: ../../../../../thirdparty/libOPNMIDI/src/opnmidi_private.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling opnmidi_private.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Ym2612_Emu_dd631ef0.o: ../../../../../thirdparty/libOPNMIDI/src/chips/gens/Ym2612_Emu.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Ym2612_Emu.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/mame_ym2612fm_bf0c8cbd.o: ../../../../../thirdparty/libOPNMIDI/src/chips/mame/mame_ym2612fm.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling mame_ym2612fm.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/emu2149_c4616ba2.o: ../../../../../thirdparty/libOPNMIDI/src/chips/mamefm/emu2149.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling emu2149.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/fm_7c41fee0.o: ../../../../../thirdparty/libOPNMIDI/src/chips/mamefm/fm.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling fm.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/resampler_28ddff46.o: ../../../../../thirdparty/libOPNMIDI/src/chips/mamefm/resampler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling resampler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ymdeltat_6208b669.o: ../../../../../thirdparty/libOPNMIDI/src/chips/mamefm/ymdeltat.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ymdeltat.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/fmgen_file_b2ae057e.o: ../../../../../thirdparty/libOPNMIDI/src/chips/np2/fmgen_file.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling fmgen_file.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/fmgen_fmgen_3d85cdb1.o: ../../../../../thirdparty/libOPNMIDI/src/chips/np2/fmgen_fmgen.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling fmgen_fmgen.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/fmgen_fmtimer_43f10826.o: ../../../../../thirdparty/libOPNMIDI/src/chips/np2/fmgen_fmtimer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling fmgen_fmtimer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/fmgen_opna_cf158b56.o: ../../../../../thirdparty/libOPNMIDI/src/chips/np2/fmgen_opna.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling fmgen_opna.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/fmgen_psg_71c63a8c.o: ../../../../../thirdparty/libOPNMIDI/src/chips/np2/fmgen_psg.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling fmgen_psg.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ym3438_c6367e33.o: ../../../../../thirdparty/libOPNMIDI/src/chips/nuked/ym3438.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ym3438.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/gens_opn2_2f9f063e.o: ../../../../../thirdparty/libOPNMIDI/src/chips/gens_opn2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling gens_opn2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/gx_opn2_c6d72e30.o: ../../../../../thirdparty/libOPNMIDI/src/chips/gx_opn2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling gx_opn2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/mame_opn2_b701b555.o: ../../../../../thirdparty/libOPNMIDI/src/chips/mame_opn2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling mame_opn2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/mame_opna_b9980604.o: ../../../../../thirdparty/libOPNMIDI/src/chips/mame_opna.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling mame_opna.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/np2_opna_d8992a70.o: ../../../../../thirdparty/libOPNMIDI/src/chips/np2_opna.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling np2_opna.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/nuked_opn2_878260ae.o: ../../../../../thirdparty/libOPNMIDI/src/chips/nuked_opn2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling nuked_opn2.cpp"
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

clean:
	@echo Cleaning OPNplug
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping OPNplug
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_VST:%.o=%.d)
-include $(OBJECTS_STANDALONE_PLUGIN:%.o=%.d)
-include $(OBJECTS_SHARED_CODE:%.o=%.d)
