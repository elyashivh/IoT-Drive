#include <iostream>
#include <csignal>
#include <exception>
#include <ratio>

#include "Scheduler.hpp"

using namespace std;

namespace ilrd_166_7
{
/****FORWARD DECLARATION AND CONST VARS****/
static const size_t MICRO = 1000000;
static const size_t MILLI = 1000;
static void Execute(union sigval sv);

struct TaskWrapper
{
    shared_ptr<Scheduler::ISchedulerTask> task;
};

/******CLASS METHODE IMPLEMENTATION****/

void Scheduler::AddTask(std::shared_ptr<ISchedulerTask> task, std::chrono::milliseconds ms)
{
    timer_t timerId;
    struct sigevent sev{};

    auto* wrapper = new TaskWrapper{task};

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function =  &Execute;
    sev.sigev_value.sival_ptr = wrapper;

    if (timer_create(CLOCK_REALTIME, &sev, &timerId) == -1) 
    {
        delete wrapper;
        throw runtime_error("timer_create failed");
    }

    struct itimerspec its {};
    its.it_value.tv_sec = ms.count() / MILLI;
    its.it_value.tv_nsec = (ms.count() % MILLI) * MICRO;

    if (timer_settime(timerId, 0, &its, nullptr) == -1) 
    {
        delete wrapper;
        throw runtime_error("timer_settime failed");
    }
}

static void Execute(union sigval sv)
{
    auto* wrapper = static_cast<TaskWrapper*>(sv.sival_ptr);
    wrapper->task->Execute();
    delete wrapper;
}
}   //end namespace ilrd_166_7