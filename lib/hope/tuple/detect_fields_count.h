/* Copyright (C) 2020 - 2022 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <utility>

/*! \defgroup <reflection> Static reflection
    @{
    \file
    \brief This file consists of helper functions which is used to compute fields count (in general)
    And the only one function which computes count of the structure's fields (non - recursive)
*/

namespace hope {
    namespace detail {

        /**
         * The only one reason of existing of this class is initialization of object of different types.
         * This is the same as ubiq from magic-get
         * @tparam TStruct Type to be converted to
         * @tparam I Index, is used to unambiguously define the type
         */
        template <typename TStruct, std::size_t I>
        struct any_convertible {
            template <typename T, typename = std::enable_if_t<!std::is_base_of_v<T, TStruct>>>
            constexpr operator T& () const noexcept;
        };

        /**
         * This two functions is needed to determine whether object of corresponding type might be constructed with
         * specified count of objects or not. NOTE: uses aggregate initialization, only aggregate initialisable
         * classes are allowed.
         * @tparam TStruct type of the structure to check
         * @tparam Is Sequence of indexes to initialize
         * @return true if the type is initialisable with sizeof(Is...) objects
         */
        template <typename TStruct, std::size_t... Is>
        constexpr auto is_constructable_n(std::index_sequence<Is...>)
            ->decltype(TStruct{ any_convertible<TStruct, Is>{}... }, bool()) {
            return true;
        }
        template <typename TStruct, std::size_t... Is>
        constexpr auto is_constructable_n(...) {
            return false;
        }

        /**
         * Implementation of the fields computation function.
         * @tparam TStructure Desired type
         * @tparam Is Indexes of the sequence
         * @param sequence Helper sequence of the indexes which might be used to compute fields count
         * @return Count of structure's fields
         */
        template <typename TStructure, std::size_t... Is>
        constexpr std::size_t detect_fields_count_impl(std::index_sequence<Is...> sequence) {
            bool bs[] = { is_constructable_n<TStructure>(std::make_index_sequence<Is>())... };
            // since cxx20... msvc sucks
            //return std::distance(std::begin(bs), std::find(
            //    std::begin(bs), std::end(bs), false)
            //);

            for (std::size_t i{ 1 }; i < sequence.size(); ++i) {
                if (!bs[i]) return i - 1;
            }
            return 0;
        }
    }

    /**
     * Interface of computation function, is intended to catch all the errors and show error reason.
     * @tparam TStructure Desired type to compute fields.
     * @return Number of fields
     */
    template <typename TStructure>
    constexpr std::size_t detect_fields_count() {
        static_assert(!std::is_polymorphic_v<TStructure>, "---- hope ---- Polymorphic types are not allowed");
        static_assert(std::is_aggregate_v<TStructure>, "---- hope ---- Only aggregate initializable types are allowed");

        constexpr auto size = sizeof(TStructure);
        return detail::detect_fields_count_impl<TStructure>(std::make_index_sequence<size + 2>());
    }
}

/*! @} */
