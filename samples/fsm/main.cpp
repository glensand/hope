/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "hope/fsm/fsm.h"

struct enabled final {};
struct disabled final{};
struct change_state final{};

int main() {
    auto fsm = hope::make_fsm<enabled, disabled>(
        [](enabled, change_state) -> hope::transit_to<disabled> {
            return {};
        },
        [](disabled, change_state) -> hope::transit_to<enabled> {
            return {};
        }
        );

    std::cout << "State: " << fsm.get_cur_state() << std::endl;
    fsm.on_event(change_state{});
    std::cout << "State: " << fsm.get_cur_state() << std::endl;
    fsm.on_event(change_state{});
    std::cout << "State: " << fsm.get_cur_state() << std::endl;

	return 0;
} 