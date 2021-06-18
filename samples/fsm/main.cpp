/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/fsm/fsm.h"
#include <iostream>

struct enabled final {};
struct disabled final{};
struct change_state final{};

int main() {
    auto fsm = hope::fsm::make<enabled, disabled>(
        [](enabled, change_state) -> hope::fsm::transit_to<disabled> {
            std::cout << "Transition: Enabled -> Disabled" << std::endl;
            return {};
        },
        [](disabled, change_state) -> hope::fsm::transit_to<enabled> {
            std::cout << "Transition: Disabled -> Enabled" << std::endl;
            return {};
        }
        );

    fsm.on_event(change_state{});
    fsm.on_event(change_state{});

	return 0;
} 