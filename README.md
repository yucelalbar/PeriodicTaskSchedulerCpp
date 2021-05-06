# PeriodicTaskScheduler
A Periodic Task Scheduler with C++ and Boost.Asio
* Boost deadline_timer is used for periodic scheduler.
* boost::asio::io_service is used to execute task asyncly.
* A timer scanner is executed periodically with a given time in milliseconds granularity.
* Tasks can be added with desired fire time to timer scanner queue.
 
