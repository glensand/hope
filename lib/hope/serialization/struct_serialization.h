/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/tuple/tuple_from_struct.h"
#include "hope/serialization/package.h"
#include <bitset>
#include <utility>

namespace hope::serialization {

    class package;

    template <typename T>
    class pod_serializer final {
    public:
        pod_serializer(T& serializable_struct, double _fp_accuracy = std::numeric_limits<double>::epsilon())
            : value(serializable_struct)
            , fp_accuracy(_fp_accuracy) {}

        void serialize(package& pack) {
            auto&& tuple = tuple_from_struct(value, field_policy::reference{});
            auto&& prev_tuple = tuple_from_struct(prev_value, field_policy::reference{});

            auto&& mask = first_pack ? get_equal_mask() : compute_mask(tuple, prev_tuple, std::make_index_sequence<fields_count>());
            first_pack = false;

            write_mask(mask, pack);

            std::size_t index{ 0 };
            for_each(tuple, [&](auto&& field) {
                if (mask[index])
                    pack.write(field);
            });

            prev_value = value;
        }

        void deserialize(package& pack) {
            auto&& tuple = tuple_from_struct(value, field_policy::reference{});

            auto mask = read_mask(pack);

            std::size_t index{ 0 };
            for_each(tuple, [&](auto&& field) {
                if (mask[index]) {
                    field = pack.read<std::decay_t<decltype(field)>>();
                }
            });
        }

    private:

        template <typename V>
        [[nodiscard]] bool is_equal(const V& left, const V& right) const {
            if constexpr (std::is_floating_point_v<V>)
                return std::abs(left - right) < fp_accuracy;
            return left == right;
        }

        template <typename Tuple, std::size_t... Is>
        [[nodiscard]] auto compute_mask(const Tuple& tuple, const Tuple& prev_tuple, std::index_sequence<Is...>) const noexcept{
            bool bs[] = { !is_equal(tuple.template get<Is>(), prev_tuple.template get<Is>())... };
            return std::bitset<fields_count>(bs);
        }

        [[nodiscard]] auto get_equal_mask() const {
            return std::bitset<fields_count>(true);
        }

        template <std::size_t Size>
        static void write_mask(const std::bitset<Size>& mask, package& pack) {
            if constexpr (Size <= std::numeric_limits<unsigned long>::digits)
                pack.write(mask.to_ulong());
            else
                pack.write(mask.to_ullong());
        }

        template<typename MaskType>
        static auto read_mask_impl(package& pack) {
            auto&& mask = pack.read<MaskType>();
            return std::bitset<fields_count>(mask);
        }

        static auto read_mask(package& pack) {
            if constexpr (fields_count <= std::numeric_limits<unsigned long>::digits)
                return read_mask_impl<unsigned long>(pack);
            return read_mask_impl<unsigned long long>(pack);
        }

        T& value;
        T prev_value;
        bool first_pack{ true };

        const double fp_accuracy;
        constexpr static std::size_t fields_count{ detect_fields_count(T{}) };

        static_assert(fields_count <= std::numeric_limits<unsigned long long>::digits, "Men, you can't use this serializer for structure with 65 fields and more.");
    };

}