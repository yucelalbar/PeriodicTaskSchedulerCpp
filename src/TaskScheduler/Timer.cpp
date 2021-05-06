/**
 * Timer.cpp
 */

#include "Timer.h"

namespace TaskScheduler {

Timer::Timer(boost::asio::io_service& ioService, int32_t timerMilliSecs)
            : m_timer(ioService)
            , m_timerMilliSecs(timerMilliSecs) {

}

void Timer::start(boost::function<void(const boost::system::error_code&)> cb) {

    m_timer.expires_from_now(boost::posix_time::milliseconds(m_timerMilliSecs));
    m_timer.async_wait(cb);
}

void Timer::cancel() {
    try {
        m_timer.cancel();
    }
    catch(...) {

    }
}


} // namespace TaskScheduler


