/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/tuple/tuple_from_struct.h"
#include "hope/tuple/compute_field_count_recursive.h"
#include "hope/serialization/package.h"
#include <bitset>
#include <utility>

namespace hope::serialization {

    class package;

    template <typename T>
    class pod_serializer final {
        constexpr static std::size_t fields_count{ compute_field_count_recursive_constexpr<T>() };
        using bit_mask = std::bitset <fields_count>;
    public:
        pod_serializer(T& serializable_struct, double _fp_accuracy = std::numeric_limits<double>::epsilon())
            : m_value(serializable_struct)
            , m_fp_accuracy(_fp_accuracy) {}

        void serialize(package& pack) {
            const auto mask_begin_pos = pack.reserve(get_bit_mask_actual_size());
            std::size_t index{ 0 };
            serialize(m_value, m_prev_value, pack, index);
            const auto struct_end_pos = pack.seek(mask_begin_pos);
            write_mask( pack);
            pack.seek(struct_end_pos);
            m_first_pack = false;
            m_prev_value = m_value;
        }

        void deserialize(package& pack) {
            read_mask(pack);
            std::size_t index{ 0 };
            deserialize(m_value, pack, index);
        }

    private:
        template<typename TValue>
        std::enable_if_t<std::is_class_v<TValue>>
        serialize(const TValue& value, const TValue& prev_value, package& pack, std::size_t& cur_index) {
            auto&& tuple = tuple_from_struct(value, field_policy::reference{});
            auto&& prev_tuple = tuple_from_struct(prev_value, field_policy::reference{});

            for_each(tuple, prev_tuple, [&](auto&& field, auto&& prev_field) {
                serialize(field, prev_field, pack, cur_index);
            });
        }

        template<typename TValue>
        std::enable_if_t<!std::is_class_v<TValue>>
        serialize(const TValue& value, const TValue& prev_value, package& pack, std::size_t& cur_index) {
            const bool changed = m_first_pack || !is_equal(value, prev_value);
            if (changed)
                pack.write(value);
            m_mask[cur_index] = changed;
            ++cur_index;
        }

        template<typename TValue>
        std::enable_if_t<std::is_class_v<TValue>>
        deserialize(TValue& value, package& pack, std::size_t& cur_index) {
            auto&& tuple = tuple_from_struct(value, field_policy::reference{});

            for_each(tuple, [&](auto&& field) {
                deserialize(field, pack, cur_index);
            });
        }

        template<typename TValue>
        std::enable_if_t<!std::is_class_v<TValue>>
        deserialize(TValue& value, package& pack, std::size_t& cur_index) {
            if (m_mask[cur_index])
                value = pack.read<TValue>();
            ++cur_index;
        }

        template <typename V>
        [[nodiscard]] bool is_equal(const V& left, const V& right) const {
            if constexpr (std::is_floating_point_v<V>)
                return std::abs(left - right) < m_fp_accuracy;
            return left == right;
        }

        void write_mask(package& pack) {
            if constexpr (fields_count <= std::numeric_limits<unsigned long>::digits)
                pack.write(m_mask.to_ulong());
            else
                pack.write(m_mask.to_ullong());
        }

        template<typename MaskType>
        void read_mask_impl(package& pack) {
            auto&& mask = pack.read<MaskType>();
            m_mask = bit_mask(mask);
        }

        void read_mask(package& pack) {
            if constexpr (fields_count <= std::numeric_limits<unsigned long>::digits)
                read_mask_impl<unsigned long>(pack);
            else
                read_mask_impl<unsigned long long>(pack);
        }

        [[nodiscard]] static constexpr auto get_bit_mask_actual_size() {
            if constexpr (fields_count <= std::numeric_limits<unsigned long>::digits)
                return sizeof (unsigned long);
            return sizeof(unsigned long long);
        }

        T& m_value;
        T m_prev_value;
        bool m_first_pack{ true };
        bit_mask m_mask;
        const double m_fp_accuracy;

        static_assert(fields_count <= std::numeric_limits<unsigned long long>::digits, "Men, you can't use this serializer for structure with 65 fields and more.");
    };

}