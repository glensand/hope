/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include "hope/tuple/flat_tuple.h"
#include <variant>

namespace hope::fsm {

    template <typename State>
    struct transit_to final {
        using target_state_t = State;
    };

    template <typename, typename... Ts>
    class fsm;

    template <typename... States, typename... Handlers>
    class fsm<flat_tuple<States...>, Handlers...> final : public Handlers... {
    public:
        using Handlers::operator()...;

        constexpr explicit fsm(Handlers&&... handlers)
            : Handlers(std::forward<Handlers>(handlers))...{ }

        template<typename Event>
        void on_event(Event&& e) {
            std::size_t state_index{ 0 };
            bool transition_done{ false };
            for_each(states, [&] (auto&& state){
                if(!transition_done && state_index == cur_state) {
                    using state_t = std::decay_t<decltype(state)>;
                    if constexpr (std::is_invocable_v<fsm, state_t, Event&&>) {
                        using result_t = std::invoke_result_t<fsm, state_t, Event&&>;
                        auto transition_res = this->operator()(state, std::forward<Event>(e));
                        (void)transition_res; // remove unused variable warning (var is handled 'cause it may be used later)
                        if constexpr (!std::is_same_v<result_t, void>) { // move to next state
                            cur_state = find<typename result_t::target_state_t>(type_list<States...>{});
                            transition_done = true;
                        }
                    }
                }
                ++state_index;
            });
        }

        [[nodiscard]] std::size_t get_cur_state() const noexcept {
            return cur_state;
        }

    private:
        flat_tuple<States...> states;
        std::size_t cur_state{ 0 };
    };

    template <typename... States, typename... Handlers>
    constexpr auto make(Handlers&&... handlers) {
        return fsm<flat_tuple<States...>, Handlers...>(std::forward<Handlers>(handlers)...);
    }

}
