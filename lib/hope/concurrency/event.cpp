#include "hope/concurrency/event.h"

#if defined(_WIN32) || defined(_WIN64)
#include "Windows.h"
#endif

namespace hope::concurrency {

    synchronization_event::synchronization_event(policy policy) noexcept {
#if defined(_WIN32) || defined(_WIN64)
        m_event = CreateEvent(nullptr, policy != policy::Auto, 
            false, nullptr);
#endif
    }

    synchronization_event::~synchronization_event() noexcept {
#if defined(_WIN32) || defined(_WIN64)
        CloseHandle(m_event);
#endif
    }

    void synchronization_event::set() const noexcept {
#if defined(_WIN32) || defined(_WIN64)
        SetEvent(m_event);
#else
        m_cv.notify_one();
#endif
    }

    void synchronization_event::reset() const noexcept {
#if defined(_WIN32) || defined(_WIN64)
        ResetEvent(m_event);
#endif
    }

    bool synchronization_event::wait(long waiting_time) const noexcept {
#if defined(_WIN32) || defined(_WIN64)
        return  WaitForSingleObject(m_event, waiting_time) != WAIT_TIMEOUT;
#else
        std::unique_lock lk(m_mutex);
        m_cv.wait(lk);
        return true;
#endif
    }

    auto_reset_event::auto_reset_event()
        : synchronization_event(policy::Auto) {

    }

    auto_reset_event::~auto_reset_event() {

    }

    manual_reset_event::manual_reset_event()
        : synchronization_event(policy::Manual) {

    }

    manual_reset_event::~manual_reset_event() {

    }
}
