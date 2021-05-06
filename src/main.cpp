#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "TaskScheduler/TimerScanner.h"



class OperationX {
    public:
        void execute() {
            std::cout << "OperationX executed" << std::endl;
        }
};

class OperationY {
    public:
        void execute(int32_t x, const std::string& s) {
            std::cout << "OperationY executed. x: " << x << " s: " << s << std::endl;
        }
};


int main(int argc, char* argv[])
{
    boost::asio::io_service service;

    TaskScheduler::TimerScanner scanner{service, 10}; //scanner runs every 10 milliseconds

    auto task1 = std::make_shared<OperationX>();
    auto task1Cb = boost::bind(&OperationX::execute, std::move(task1));
    auto task2 = std::make_shared<OperationY>();
    auto task2Cb = boost::bind(&OperationY::execute, std::move(task2), 15, "string");
    auto task3 = std::make_shared<OperationY>();
    auto task3Cb = boost::bind(&OperationY::execute, std::move(task2), 50, "mystring");

    scanner.addTask("1", task1Cb, 1000); //task1 will be executed after 1000 ms
    scanner.addTask("2", task2Cb, 2000); //task2 will be executed after 2000 ms
    scanner.addTask("3", task3Cb, 3000); //task3 will be executed after 3000 ms

    scanner.start();

    service.run();

    return 0;
}