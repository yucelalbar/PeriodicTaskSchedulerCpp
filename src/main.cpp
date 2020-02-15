#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>


class Task {
    public:
        void execute() {
            std::cout << "Task executed" << std::endl;
        }
};

class Timer {
    public:
        Timer(boost::asio::io_service& ioService, const int32_t& timerMilliSecs)
            : m_timer(ioService)
            , m_timerMilliSecs(timerMilliSecs) {

        }

        void start(boost::function<void(const boost::system::error_code&)> cb) {
            m_timer.expires_from_now(boost::posix_time::milliseconds(m_timerMilliSecs));
            m_timer.async_wait(cb);
        }

    private:
        boost::asio::deadline_timer m_timer;
        int32_t m_timerMilliSecs;
};

class TimerScanner {
    public:
        TimerScanner(boost::asio::io_service& ioService, const int32_t& timerGranularity)
                    : m_ioService(ioService) {
            m_timer = std::make_unique<Timer>(ioService, timerGranularity);
            m_cb = boost::bind(&TimerScanner::onTimerExec, this, _1);
        }
        void start() {
            m_timer->start(m_cb);
        }

        void addTask(const boost::function<void()>& task) {
            m_tasks.push_back(task);
        }
        void onTimerExec(const boost::system::error_code& ec)
        {
            if (ec != boost::asio::error::operation_aborted) {
                start();
                /*Execute tasks asynchronously*/
                for(const auto& task : m_tasks) {
                    m_ioService.post(task);
                }
                /*Clear tasks list for next onTimerExec*/
                m_tasks.clear();

            }
        }

    private:
        boost::asio::io_service& m_ioService;
        std::vector<boost::function<void()>> m_tasks;
        std::unique_ptr<Timer> m_timer;
        boost::function<void(const boost::system::error_code& ec)> m_cb;
};


int main(int argc, char* argv[])
{
    boost::asio::io_service service;
    int32_t millisecs = 1000;

    TimerScanner scanner(service, millisecs);

    auto task1 = std::make_shared<Task>();
    auto task1Cb = boost::bind(&Task::execute, task1);
    auto task2 = std::make_shared<Task>();
    auto task2Cb = boost::bind(&Task::execute, task2);
    scanner.addTask(task1Cb);
    scanner.addTask(task2Cb);

    scanner.start();

    service.run();

    return 0;
}