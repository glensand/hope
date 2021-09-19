/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <fstream>
#include <string_view>
#include "hope/stream/ostream.h"

namespace hope {

    class ofstream final : public ostream{
    public:

        ofstream(std::string_view file_name);
        virtual ~ofstream() override;

        DECLARE_EXPLICIT_DEFAULT_MOVABLE(ofstream);
        DECLARE_NON_COPYABLE(ofstream);

        virtual bool is_open() const noexcept override;

        virtual void write(const void* data, std::size_t size) override;

        virtual void flush() override;

    private:
        std::ofstream m_stream_impl;
    };

}
