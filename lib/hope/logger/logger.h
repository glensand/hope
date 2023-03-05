/* Copyright (C) 2021 - 2023 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#pragma once

#include <string>
#include <atomic>
#include <thread>

#include "hope/foundation.h"
#include "hope/concurrency/event.h"
#include "hope/logger/log_level.h"

namespace hope {

    class stack_buffer;
    class ostream;

    class logger final {
    public:
        DECLARE_NON_MOVABLE(logger);
        DECLARE_NON_COPYABLE(logger);

	    logger() = default;
	    ~logger() noexcept;

	    bool should_write(log_priority priority) const noexcept;

        void enable(ostream& stream, log_level level) noexcept;

    private:
	    void add(const stack_buffer& buffer);
	    void flush_task();

	    std::thread m_writing_thread;
	    hope::concurrency::auto_reset_event m_event_added;
        std::atomic_bool m_enabled = false;
        ostream* m_stream = nullptr;
        log_level m_log_level = log_level::debug;

	    friend struct log_helper;
    };

}
