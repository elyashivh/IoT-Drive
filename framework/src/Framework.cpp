#include "Framework.hpp"
#include <iostream>

using namespace std;

namespace ilrd_166_7
{

    AsyncInjectionFunction::AsyncInjectionFunction(chrono::milliseconds interval) : m_interval(interval) {}

    std::chrono::milliseconds AsyncInjectionFunction::GetInterval() const
    {
        return m_interval;
    }

    /******Framework IMPLEMENTATION********* */
    Framework::Framework(IListener *listner,
                         std::list<std::pair<fdAndMode, IInputProxy *>> reactorArgs,
                         std::list<std::pair<key_t, creator_t>> factoryArgs,
                         std::string pluginPathName) : m_reactor(listner)
    {
        for (auto &arg : reactorArgs)
        {
            InputMediator cb(arg.second, arg.first);
            m_reactor.Register(arg.first.first, arg.first.second, cb);
        }
        auto factory = Handleton<Factory<ICommand, int>>::GetInstance();
        for (auto &arg : factoryArgs)
        {
            factory->Add(arg.first, arg.second);
        }
    }

    void Framework::Run()
    {
        Singleton<ThreadPool>::GetInstance()->Run();
        m_reactor.Run();
    }

    /******InputMediator IMPLEMENTATION********* */
    InputMediator::InputMediator(IInputProxy *proxy, fdAndMode fdMode) : m_pInputProxy(proxy), m_fdMode(fdMode) { /*empty*/ }
    void InputMediator::operator()(int fd, Mode mode)
    {
        std::shared_ptr<IKeyArgs> args = m_pInputProxy->GetKeyArgs(m_fdMode);
        shared_ptr<ThreadPool::ITask> currTask(new TPTask(args));
        ThreadPool::Priority currPriority = ThreadPool::Priority::MEDIUM;

        Singleton<ThreadPool>::GetInstance()->AddTask(currTask, currPriority);
    }

    /******TPTask IMPLEMENTATION********* */
    TPTask::TPTask(std::shared_ptr<IKeyArgs> args) : m_args(args)
    {
        /*empty*/
    }

    void TPTask::Run()
    {
        auto factory = Handleton<Factory<ICommand, int>>::GetInstance();
        std::shared_ptr<ICommand> newCmd = factory->Create(m_args->GetKey());

        asyncInjectionFunctor aif = newCmd->Run(m_args);
        if (aif)
        {
            auto lambda = [fn = std::move(aif)]()
            { return fn(); };

            int interval_int = 10;
            std::chrono::milliseconds interval(interval_int);
            new AsyncInjection(lambda, interval);
        }
    }

} // end namespace ilrd_166_7