//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include <vector>

struct Km_Skin;
typedef boost::intrusive_ptr<Km_Skin> Km_Skin_Ptr;
typedef boost::intrusive_ptr<const Km_Skin> Km_Skin_CPtr;

struct Km_Skin : public boost::intrusive_ref_counter<Km_Skin>
{
    std::vector<Image> frames;
    explicit operator bool() const { return !frames.empty(); }

    void load(const Image &img, unsigned frame_count);
    void load_data(const char *data, unsigned size, unsigned frame_count);
    void load_resource(const char *name, unsigned frame_count);

    Km_Skin_Ptr scaled(double ratio) const;
};
