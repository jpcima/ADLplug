//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "player.h"
#include <stdexcept>

void Player::init(unsigned sample_rate)
{
    ADL_MIDIPlayer *pl = adl_init(sample_rate);
    if (!pl)
        throw std::runtime_error("cannot initialize player");
    player_.reset(pl);
}

bool Player::set_num_4ops(unsigned count)
{
    ADL_MIDIPlayer *pl = player_.get();
    if (count == ~0u) {
        // set automatic count
        if (adl_setNumFourOpsChn(player_.get(), count) < 0)
            return false;
        // get the fixed count and set it, so it doesn't remain automatic
        count = adl_getNumFourOpsChnObtained(player_.get());
    }
    return adl_setNumFourOpsChn(player_.get(), count) >= 0;
}

void Player::play_midi(const uint8_t *msg, unsigned len)
{
    ADL_MIDIPlayer *pl = player_.get();

    if (len <= 0)
        return;

    uint8_t status = msg[0];
    if ((status & 0xf0) == 0xf0)
        return;

    uint8_t channel = status & 0x0f;
    switch (status >> 4) {
    case 0b1001:
        if (len < 3) break;
        if (msg[2] != 0) {
            adl_rt_noteOn(pl, channel, msg[1], msg[2]);
            break;
        }
    case 0b1000:
        if (len < 3) break;
        adl_rt_noteOff(pl, channel, msg[1]);
        break;
    case 0b1010:
        if (len < 3) break;
        adl_rt_noteAfterTouch(pl, channel, msg[1], msg[2]);
        break;
    case 0b1101:
        if (len < 2) break;
        adl_rt_channelAfterTouch(pl, channel, msg[1]);
        break;
    case 0b1011:
        if (len < 3) break;
        adl_rt_controllerChange(pl, channel, msg[1], msg[2]);
        break;
    case 0b1100:
        if (len < 2) break;
        adl_rt_patchChange(pl, channel, msg[1]);
        break;
    case 0b1110:
        if (len < 3) break;
        adl_rt_pitchBendML(pl, channel, msg[2], msg[1]);
        break;
    }
}

void Player::generate(float *left, float *right, unsigned nframes, unsigned stride)
{
    ADLMIDI_AudioFormat format;
    format.type = ADLMIDI_SampleType_F32;
    format.containerSize = sizeof(float);
    format.sampleOffset = stride * sizeof(float);
    adl_generateFormat(
        player_.get(), 2 * nframes, (uint8_t *)left, (uint8_t *)right, &format);
}

std::vector<std::string> Player::enumerate_emulators()
{
    std::unique_ptr<ADL_MIDIPlayer, Player_Deleter> pl(adl_init(44100));
    if (!pl)
        throw std::runtime_error("cannot initialize player");

    std::vector<std::string> names(32);
    size_t count = 0;
    for (unsigned i = 0, n = (unsigned)names.size(); i < n; ++i) {
        if (adl_switchEmulator(pl.get(), i) == 0) {
            names[i] = adl_chipEmulatorName(pl.get());
            count = i + 1;
        }
    }

    names.resize(count);
    return names;
}
