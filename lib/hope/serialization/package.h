/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <vector>

namespace hope::serialization {

    class package final {
        using buffer_t = std::vector<unsigned char>;
    public:
        package() = default;

        /**
         * \brief writes data to the inner buffer, automatically enlarge size of them
         * \tparam T type of the pod to be written
         * \param val given value
         */
        template <typename T>
        void write(const T& val) {
            write(&val, sizeof val);
        }

        /**
         * \brief writes sequence of raw data to the inner buffer, automatically enlarge those if it is required
         * \param data pointer to very beginning of the memory segment to be written
         * \param count count of BYTES in the memory segment
         */
        void write(const void* data, std::size_t count);

        /**
         * \brief reads object of specified type, the type should be POD
         * \tparam T type to be read
         * \return result object
         */
        template <typename T>
        T read() {
            T val;
            read(&val, sizeof val);
            return val;
        }

        /**
         * \brief reads specified amount of data to the given buffer, throws exception if buffer size is 
         * \param data pointer to very beginning of read buffer
         * \param count count of bytes to be read
         */
        void read(void* data, std::size_t count);

        /**
         * \brief 
         * \param position 
         * \return 
         */
        std::size_t seek(std::size_t position);

        /**
         * \brief 
         * \param count 
         * \return 
         */
        std::size_t reserve(std::size_t count);

        /**
         * \brief 
         * \return 
         */
        [[nodiscard]] const buffer_t& get_buffer() const noexcept { return buffer; }

        /**
         * \brief 
         */
        void clear();
    private:
        buffer_t buffer;
        std::size_t write_pos{ 0u };
        std::size_t read_pos{ 0u };
    };

}
