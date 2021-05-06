/**
 * Timer.h
 */
#ifndef _TaskScheduler_Timer_h_
#define _TaskScheduler_Timer_h_

#include <boost/asio.hpp>
#include <boost/function.hpp>

namespace TaskScheduler {

class Timer {
public:
    Timer(boost::asio::io_service& ioService, int32_t timerMilliSecs);

    void start(boost::function<void(const boost::system::error_code&)> cb);

    void cancel();

private:
    boost::asio::deadline_timer m_timer;
    int32_t m_timerMilliSecs;
};

} // namespace TaskScheduler

#endif //_TaskScheduler_Timer_h_