/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <cassert>

#include "hope/components/singleton_holder/creation_model.h"
#include "hope/components/singleton_holder/threading_model.h"
#include "hope/components/singleton_holder/lifetime_model.h"

namespace hope {

    template<
        typename SingletonImpl,
        template <typename> typename CreationModel = create_static,
        template <typename> typename ThreadingModel = single_threaded,
        template <typename> typename LifetimeModel = lifetime_auto
    >
    class singleton_holder final {
    public:

        singleton_holder() = default;
        singleton_holder(singleton_holder&&) = delete;
        singleton_holder& operator=(singleton_holder&&) = delete;

        static SingletonImpl& instance() {
            if(m_instance == nullptr)
                initialize();
            return *m_instance;
        }

    private:
        using VolatileType = typename ThreadingModel<SingletonImpl>::VolatileType;
        using Lock = typename ThreadingModel<SingletonImpl>::Lock;

        ~singleton_holder() = default;

        static void initialize() {
            const Lock lock{ };
            if (m_instance == nullptr) {
                if (m_destroyed) {
                    LifetimeModel<SingletonImpl>::on_dead_reference();
                    m_destroyed = false;
                }
                m_instance = CreationModel<SingletonImpl>::create();
                LifetimeModel<SingletonImpl>::register_deleter(&destroy);
            }
            (void)lock;
        }
        
        static void destroy() {
            assert(!m_destroyed);
            CreationModel<SingletonImpl>::destroy(m_instance);
            m_instance = nullptr;
            m_destroyed = false;
        }

        inline static VolatileType* m_instance{ nullptr };
        inline static bool m_destroyed{ false };

        friend class LifetimeModel<SingletonImpl>;
    };

}
