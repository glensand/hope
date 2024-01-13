/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <array>
#include <string_view>
#include <vector>

#include "hope/foundation.h"

namespace hope {

    class stack_buffer final
    {
    public:
        // Size in bytes of the inner buffer this value assumed to be never reached
	    static constexpr std::size_t BufferSize = 1023;
	    using buffer_t = std::array<char, BufferSize>;
	
	    stack_buffer() = default;
        ~stack_buffer() = default;

        DECLARE_NON_MOVABLE(stack_buffer);
        DECLARE_NON_COPYABLE(stack_buffer);

	    void put(const void* data, std::size_t size) noexcept;

        std::size_t bytes_written = 0;
        buffer_t buffer;
        std::vector<char> additional_buffer;
    };

}
