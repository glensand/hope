#include "logger.h"
#include "hope/stream/ostream.h"
#include "hope/logger/stack_buffer.h"

namespace hope {

    logger::~logger() noexcept {
        if(!m_enabled.load(std::memory_order_acquire))
            return;
        try {
            m_stream->write("\0", 1);
            m_event_added.set();
            m_enabled.store(false, std::memory_order_release);
            m_writing_thread.join();
        } catch(...) {
            // Ignore
        }
    }

    bool logger::should_write(log_priority priority) const noexcept {
        if(m_log_level == log_level::minimal && priority == log_priority::info)
            return false;

        return m_enabled.load(std::memory_order_acquire);
    }

    void logger::enable(ostream &stream, log_level level) noexcept {
        m_stream = &stream;
        m_log_level = level;
        m_enabled.store(true, std::memory_order_release);
        m_writing_thread = std::thread([this]{ flush_task();});
    }

    void logger::add(const stack_buffer &buffer) {
        // NOTE!: Do not copy buffer to the separate thread, assumed that entire std::ofstream buffer is large enough
        // to contain all recorded text
        m_stream->write(buffer.buffer.data(), buffer.bytes_written);
        m_stream->write(buffer.additional_buffer.data(), buffer.additional_buffer.size());
        // Flush operation is the most complicated one, we have to perform it at the special thread
        m_event_added.set();
    }

    void logger::flush_task() {
        while(m_enabled.load(std::memory_order_acquire)) {
            m_event_added.wait();
            m_stream->flush();
        }
    }

}