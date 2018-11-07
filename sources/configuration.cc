//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "configuration.h"
#include "ui/utility/key_maps.h"
#include <SimpleIni.h>

#if 0
#   define trace(fmt, ...)
#else
#   define trace(fmt, ...) fprintf(stderr, "[Configuration] " fmt "\n", ##__VA_ARGS__)
#endif

static constexpr long config_version = 1;
static void create_default_configuration(CSimpleIniA &ini);

struct Configuration::Opaque_Ini {
    CSimpleIniA instance;
};

Configuration::Configuration()
    : ini_(new Opaque_Ini)
{
    ini_->instance.SetUnicode();
}

Configuration::~Configuration()
{
}

File Configuration::system_file_path()
{
    File file;
#if JUCE_LINUX
    file = File("/etc/" JucePlugin_Name "/" JucePlugin_Name ".ini");
#endif
    return file;
}

File Configuration::user_file_path()
{
    File data_dir = File::getSpecialLocation(File::userApplicationDataDirectory);
    return data_dir.getChildFile(JucePlugin_Manufacturer "/" JucePlugin_Name ".ini");
}

bool Configuration::load_default()
{
    Opaque_Ini_Ptr ini_default(new Opaque_Ini);

    File user = user_file_path();
    File sys = system_file_path();

    if (ini_default->instance.LoadFile(sys.getFullPathName().toRawUTF8()) != SI_OK) {
        create_default_configuration(ini_default->instance);
    }
    else {
        long version = ini_default->instance.GetLongValue("", "configuration-version");
        if (version < config_version) {
            fprintf(stderr, "!! " JucePlugin_Name " configuration: the system version (%lu) does not match the software version (%lu)!\n",
                    version, config_version);
            create_default_configuration(ini_default->instance);
        }
    }

    Opaque_Ini_Ptr ini_user(new Opaque_Ini);
    if (ini_user->instance.LoadFile(user.getFullPathName().toRawUTF8()) != SI_OK) {
        ini_ = std::move(ini_default);
    }
    else {
        long version = ini_user->instance.GetLongValue("", "configuration-version");
        if (version < config_version) {
            // use the latest configuration, keep a backup of the previous one
            ini_ = std::move(ini_default);
            File backup = user.withFileExtension("ini.bak" + String(version));
            user.moveFileTo(backup);
            save_default();
        }
        else
            ini_ = std::move(ini_user);
    }

    return false;
}

bool Configuration::save_default()
{
    File user = user_file_path();
    trace("Attempt to save '%s'", user.getFullPathName().toRawUTF8());
    return save_file(user);
}

bool Configuration::load_file(const File &file)
{
    return ini_->instance.LoadFile(file.getFullPathName().toRawUTF8()) == SI_OK;
}

bool Configuration::save_file(const File &file)
{
    file.getParentDirectory().createDirectory();
    return ini_->instance.SaveFile(file.getFullPathName().toRawUTF8()) == SI_OK;
}

void Configuration::set_string(const char *section, const char *key, const char *value)
{
    ini_->instance.SetValue(section, key, value, nullptr, true);
}

const char *Configuration::get_string(const char *section, const char *key, const char *default_value) const
{
    return ini_->instance.GetValue(section, key, default_value);
}

static void create_default_configuration(CSimpleIniA &ini)
{
    ini.Reset();

    ini.SetValue("", "configuration-version", std::to_string(config_version).c_str(),
                 "# the version of the file specification");

    ini.SetValue("paths", "last-instrument-directory", "",
                 "# the last directory in which instruments have been accessed");

    ini.SetValue("piano", "layout",
                 key_layout_names[(unsigned)Key_Layout::Default],
                 "# the default key layout");

    for (unsigned i = 0; i < key_layout_names.size(); ++i) {
        std::string layout_name = key_layout_names[i];
        std::string key = "keymap:" + layout_name;

        for (char &c : layout_name) {
            if (c >= 'a' && c <= 'z')
                c = c - 'a' + 'A';
        }
        std::string comment = "# the " + layout_name + " key map";
        ini.SetValue(
            "piano", key.c_str(),
            String(CharPointer_UTF32((const juce_wchar *)key_layout_maps[i])).toRawUTF8(),
            comment.c_str());
    }
}
