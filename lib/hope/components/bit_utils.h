/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <vector>

namespace hope::bit_utils {

    constexpr unsigned bit_count(unsigned value) {
        unsigned result = 0;

        while (value) {
            value &= (value - 1);
            ++result;
        }

        return result;
    }

    class bit_mask final {
    public:
        bit_mask(std::size_t bit_count) {
            auto bytes_count = bit_count / 8;
            if (bytes_count * 8 < bit_count)
                ++bytes_count;
            m_mask.resize(bytes_count, 0);
        }

        void clear() {
            for (auto& b : m_mask)
                b = 0;
        }

        void set(std::size_t i, bool val) {
            if(!val) return;
            const auto byte_index = i / 8;
            const auto bit_index = i - byte_index * 8;
            m_mask[byte_index] |= 1 << bit_index;
        }

        bool get(std::size_t i) {
            const auto byte_index = i / 8;
            assert(byte_index < m_mask.size());
            const auto bit_index = i - byte_index * 8;
            const auto byte = m_mask[byte_index];
            return byte & 1 << bit_index;
        }

        [[nodiscard]] std::size_t bytes_count() const {
            return m_mask.size();
        }

        [[nodiscard]] const uint8_t* data() const {
            return m_mask.data();
        }

    private:
        std::vector<uint8_t> m_mask;
    };

}
