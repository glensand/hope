#include "concurrency/event.h"
#include "Windows.h"

namespace hope::concurrency {

    synchronization_event::synchronization_event(policy policy) noexcept {
        m_event = CreateEvent(nullptr, policy != policy::Auto, 
            false, nullptr);
    }

    synchronization_event::~synchronization_event() noexcept {
        CloseHandle(m_event);
    }

    void synchronization_event::set() const noexcept {
        SetEvent(m_event);
    }

    void synchronization_event::reset() const noexcept {
        ResetEvent(m_event);
    }

    bool synchronization_event::wait(long waiting_time) const noexcept {
        return  WaitForSingleObject(m_event, waiting_time) != WAIT_TIMEOUT;
    }
}
