#include "AsyncInjection.hpp"
#include "Handleton.hpp"

using namespace std;

namespace ilrd_166_7
{
    AsyncInjection::AsyncInjectionTask::AsyncInjectionTask(AsyncInjection& asyncInjection):
                                    m_asyncInjection (asyncInjection)
    {
        //empty
    }

    void AsyncInjection::AsyncInjectionTask::Execute()
    {
        m_asyncInjection.PerformAction();
    }

    AsyncInjection::AsyncInjection(std::function<bool()> func, std::chrono::milliseconds ms):
    m_func(func), m_interval(ms), m_task(new AsyncInjectionTask(*this))
    {
        Handleton<Scheduler>::GetInstance()->AddTask(shared_ptr<AsyncInjectionTask>(m_task), m_interval);
    }

    void AsyncInjection::PerformAction()
    {

        if (m_func())
        {
            Handleton<Scheduler>::GetInstance()->AddTask(m_task, m_interval);
        }
        else
        {
            delete this;
        }
    }

}

