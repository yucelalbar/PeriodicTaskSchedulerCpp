/**
 * TimerScanner.h
 *
 */

#ifndef _TaskScheduler_TimerScanner_h_
#define _TaskScheduler_TimerScanner_h_

#include <iostream>
#include <memory>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include "Timer.h"

namespace TaskScheduler {

class TimerScanner {
public:
    TimerScanner(boost::asio::io_service& ioService, int32_t timerMilliSecs);
    void start();
    void stop();

    void addTask(const std::string& taskId, const boost::function<void()>& task, int32_t timeToCall);
    void removeTask(const std::string& taskId);


private:
    boost::asio::io_service& m_ioService;
    mutable std::mutex m_mtx;
    Timer m_timer;
    /**
     * Key is Task id
     * Value: first param is task and second param is time To Call
     */
    std::unordered_map<std::string, std::pair<boost::function<void()>, int32_t>> m_tasks;

    void onTimerExec(const boost::system::error_code& ec);
};

} // namespace TaskScheduler

#endif //_TaskScheduler_TimerScanner_h_

