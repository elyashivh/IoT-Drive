#pragma once

#include <string> //string
#include <list>   //list
#include <memory> //shared_ptr

#include "ThreadPool.hpp"
#include "Reactor.hpp"
#include "Factory.hpp"
#include "Handleton.hpp"
#include "LoadUtils.hpp"
#include "AsyncInjection.hpp"
#include "Singleton.hpp"

namespace ilrd_166_7
{

    using asyncInjectionFunctor = std::function<bool()>;

    class IKeyArgs
    {
    public:
        virtual int GetKey() = 0; // For factory
    }; // end IKeyArgs

    class IInputProxy
    {
    public:
        virtual std::shared_ptr<IKeyArgs> GetKeyArgs(fdAndMode fdMode) = 0;
    }; // end IInputProxy

    class AsyncInjectionFunction
    {
    public:
        AsyncInjectionFunction(std::chrono::milliseconds interval);
        virtual bool operator()() = 0;
        std::chrono::milliseconds GetInterval() const;

    private:
        std::chrono::milliseconds m_interval;

    }; // end AIFunction (functor)

    class ICommand
    {
    public:
        virtual asyncInjectionFunctor Run(std::shared_ptr<IKeyArgs>) = 0;
    }; // ICommand

    using key_t = int;
    using creator_t = std::function<std::shared_ptr<ICommand>()>;

    class Framework
    {
    public:
        Framework(IListener *listner,
                  std::list<std::pair<fdAndMode, IInputProxy *>> reactorArgs,
                  std::list<std::pair<key_t, creator_t>> factoryArgs,
                  std::string pluginPathName);
        void Run();

    private:
        Reactor m_reactor;

    }; // Framework

    class InputMediator
    {
    public:
        InputMediator(IInputProxy *prox, fdAndMode fdMode);
        void operator()(int fd, Mode mode);

    private:
        IInputProxy *m_pInputProxy;
        fdAndMode m_fdMode;
    }; // InputMediator

    class TPTask : public ThreadPool::ITask
    {
    public:
        TPTask(std::shared_ptr<IKeyArgs> args);
        virtual void Run();

    private:
        std::shared_ptr<IKeyArgs> m_args;
    }; // TPTask

} // end namespace ilrd_166_7