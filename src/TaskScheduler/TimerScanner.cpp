/**
 * TimerScanner.cpp
 *
 */

#include "TimerScanner.h"

#include <boost/bind.hpp>

namespace TaskScheduler {

TimerScanner::TimerScanner(boost::asio::io_service& ioService, int32_t timerMilliSecs) : m_ioService{ioService}
                                                                                       , m_timer{ioService, timerMilliSecs} {

}

void TimerScanner::start() {
    m_timer.start(boost::bind(&TimerScanner::onTimerExec, this, _1));

}

void TimerScanner::stop() {
    m_timer.cancel();
}

void TimerScanner::addTask(const std::string& taskId, const boost::function<void()>& task, int32_t timeToCall) {
    std::lock_guard<std::mutex> lg(m_mtx);
    m_tasks[taskId] = std::make_pair(task, timeToCall);
}

void TimerScanner::removeTask(const std::string& taskId) {
    std::lock_guard<std::mutex> lg(m_mtx);
    m_tasks.erase(taskId);
}


void TimerScanner::onTimerExec(const boost::system::error_code& ec) {

    if (ec != boost::asio::error::operation_aborted) {
        start();

        std::lock_guard<std::mutex> lg(m_mtx);
        auto taskItr = m_tasks.begin();

        while(taskItr != m_tasks.end()) {

            taskItr->second.second -= 10;

            if(taskItr->second.second <= 0) {
                /*Execute tasks asynchronously*/
                m_ioService.post(taskItr->second.first);

                /*Erase task from list for next onTimerExec*/
                taskItr = m_tasks.erase(taskItr);
            }
            else {
                ++taskItr;
            }
        }
    }
}

} // namespace TaskScheduler
