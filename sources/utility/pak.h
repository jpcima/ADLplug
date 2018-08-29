//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <string>
#include <vector>
#include <cstdint>

class Pak_File_Reader {
public:
    bool init_with_data(const uint8_t *data, uint32_t size);
    uint32_t entry_count() const;

    const std::string &name(uint32_t nth) const;
    std::string extract(uint32_t nth) const;

private:
    struct Entry {
        uint32_t size = 0;
        uint32_t offset = 0;
        std::string name;
    };

    const uint8_t *data_ = nullptr;
    uint32_t size_ = 0;

    std::vector<Entry> entries_;
    size_t content_offset_ = 0;

    bool read_dictionary();
};
