#include "log_helper.h"
#include "hope/logger/logger.h"

namespace hope {

    log_helper::log_helper(logger &logger_instance, log_priority prior)
        : m_logger(logger_instance)
        , m_priority(prior) {

    }

    log_helper::~log_helper() {
        m_logger.add(m_buffer);
    }

    void log_helper::write_impl(const void *data, std::size_t size) {
        m_buffer.put(data, size);
    }

    void log_helper::write_impl(std::string_view data) {
        write_impl(data.data(), data.size());
    }

    void log_helper::write_impl(const std::string &data) {
        write_impl(data.data(), data.size());
    }

}