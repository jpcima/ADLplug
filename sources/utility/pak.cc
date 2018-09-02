//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "pak.h"
#include "JuceHeader.h"

bool Pak_File_Reader::init_with_data(const uint8_t *data, uint32_t size)
{
    data_ = data;
    size_ = size;
    return read_dictionary();
}

uint32_t Pak_File_Reader::entry_count() const
{
    return entries_.size();
}

const std::string &Pak_File_Reader::name(uint32_t nth) const
{
    const Entry &entry = entries_.at(nth);
    return entry.name;
}

std::string Pak_File_Reader::extract(uint32_t nth) const
{
    const Entry &entry = entries_.at(nth);

    MemoryInputStream mem_stream(data_ + content_offset_, size_ - content_offset_, false);
    GZIPDecompressorInputStream gz_stream(&mem_stream, false, GZIPDecompressorInputStream::gzipFormat);

    if (!gz_stream.setPosition(entry.offset))
        return std::string();

    std::string data(entry.size, '\0');
    if ((unsigned)gz_stream.read(&data[0], entry.size) != entry.size)
        return std::string();

    return data;
}

bool Pak_File_Reader::read_dictionary()
{
    entries_.clear();
    entries_.reserve(256);
    content_offset_ = 0;

    const uint8_t *ptr = data_;
    uint32_t left = size_;
    bool dictionary_end = false;
    while (!dictionary_end) {
        Entry ent;

        if (left < 4)
            return false;

        ent.size = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
        ptr += 4;
        left -= 4;

        dictionary_end = ent.size == 0;

        if (!dictionary_end) {
            if (left < 4)
                return false;

            ent.offset = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
            ptr += 4;
            left -= 4;

            ent.name.reserve(64);
            char c;
            do {
                if (left == 0)
                    return false;
                c = *ptr++;
                --left;
                if (c != '\0')
                    ent.name.push_back(c);
            } while (c != '\0');
            entries_.push_back(std::move(ent));
        }
    }

    content_offset_ = ptr - data_;
    return true;
}
