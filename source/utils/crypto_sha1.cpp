/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2022 Bernardo Giordano, Admiral Fish, piepie62
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
 *       * Requiring preservation of specified reasonable legal notices or
 *         author attributions in that material or in the Appropriate Legal
 *         Notices displayed by works containing it.
 *       * Prohibiting misrepresentation of the origin of that material,
 *         or requiring that modified versions of such material be marked in
 *         reasonable ways as different from the original version.
 */

#include "utils/crypto.hpp"

namespace pksm::crypto
{
    std::array<u8, 20> sha1(std::span<const u8> data)
    {
        SHA1 context;
        context.update(data);
        return context.finish();
    }

    void SHA1::update()
    {
        u32 w[80];
        for (size_t i = 0; i < 16; i++)
        {
            w[i] = (data[i * 4 + 0] << 24) | (data[i * 4 + 1] << 16) | (data[i * 4 + 2] << 8) |
                   (data[i * 4 + 3] << 0);
        }
        for (size_t i = 16; i < 80; i++)
        {
            w[i] = w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16];
            w[i] = std::rotl(w[i], 1);
        }

        u32 a = state[0], b = state[1], c = state[2], d = state[3], e = state[4];

        for (size_t i = 0; i < 80; i++)
        {
            u32 f, k;

            if (i < 20)
            {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            }
            else if (i < 40)
            {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if (i < 60)
            {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else if (i < 80)
            {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            u32 temp = std::rotl(a, 5) + f + e + k + w[i];
            e        = d;
            d        = c;
            c        = std::rotl(b, 30);
            b        = a;
            a        = temp;
        }

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
        state[4] += e;
    }

    void SHA1::update(std::span<const u8> buf)
    {
        for (size_t i = 0; i < buf.size(); i++)
        {
            data[dataLength++] = buf[i];
            if (dataLength == 64)
            {
                update();
                bitLength  += 512;
                dataLength = 0;
            }
        }
    }

    std::array<u8, 20> SHA1::finish()
    {
        std::array<u8, 20> ret;

        uint32_t i = dataLength;

        // Pad whatever data is left in the buffer.
        if (dataLength < 56)
        {
            data[i++] = 0x80;
            while (i < 56)
            {
                data[i++] = 0x00;
            }
        }
        else
        {
            data[i++] = 0x80;
            while (i < 64)
            {
                data[i++] = 0x00;
            }
            update();
            std::fill_n(data, 56, 0);
        }

        // Append to the padding the total message's length in bits and transform.
        bitLength += dataLength * 8;
        data[63]  = bitLength;
        data[62]  = bitLength >> 8;
        data[61]  = bitLength >> 16;
        data[60]  = bitLength >> 24;
        data[59]  = bitLength >> 32;
        data[58]  = bitLength >> 40;
        data[57]  = bitLength >> 48;
        data[56]  = bitLength >> 56;
        update();

        // Since this implementation uses little endian byte ordering and SHA uses big endian,
        // reverse all the bytes when copying the final state to the output hash.
        for (i = 0; i < 4; ++i)
        {
            ret[i]      = (state[0] >> (24 - i * 8)) & 0x000000ff;
            ret[i + 4]  = (state[1] >> (24 - i * 8)) & 0x000000ff;
            ret[i + 8]  = (state[2] >> (24 - i * 8)) & 0x000000ff;
            ret[i + 12] = (state[3] >> (24 - i * 8)) & 0x000000ff;
            ret[i + 16] = (state[4] >> (24 - i * 8)) & 0x000000ff;
        }

        return ret;
    }
}
