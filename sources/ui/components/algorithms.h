//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace Algorithms {

static const char16_t algorithm_2fm[] =
    u"┌┐  \n"
    u"│1 2\n"
    u"└┴─┘\n";

static const char16_t algorithm_2am[] =
    u"┌┐\n"
    u"│1\n"
    u"└┤\n"
    u" 2";

static const char16_t algorithm_4fmfm[] =
    u"┌┐\n"
    u"│1\n"
    u"└┤\n"
    u" 2\n"
    u" │\n"
    u" 3\n"
    u" │\n"
    u" 4";

static const char16_t algorithm_4amfm[] =
    u"    \n"
    u"   2\n"
    u"   │\n"
    u"   3\n"
    u"┌┐ │\n"
    u"│1 4\n"
    u"└┴─┘";

static const char16_t algorithm_4fmam[] =
    u"┌┐  \n"
    u"│1 3\n"
    u"└┤ │\n"
    u" 2 4\n"
    u" └─┘";

static const char16_t algorithm_4amam[] =
    u"      \n"
    u"   2  \n"
    u"┌┐ │  \n"
    u"│1 3 4\n"
    u"└┴─┴─┘";

}  // namespace Algorithms
