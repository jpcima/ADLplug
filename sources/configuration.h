//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <JuceHeader.h>

class Configuration {
public:
    static File system_file_path();
    static File user_file_path();

    Configuration();
    ~Configuration();
    bool load_default();
    bool save_default();
    bool load_file(const File &file);
    bool save_file(const File &file);

    void set_string(const char *section, const char *key, const char *value);
    const char *get_string(const char *section, const char *key, const char *default_value) const;

private:
    struct Opaque_Ini;
    typedef std::unique_ptr<Opaque_Ini> Opaque_Ini_Ptr;
    Opaque_Ini_Ptr ini_;
};
