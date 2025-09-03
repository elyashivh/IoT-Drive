/*
Author:     Elyashiv Horden
Date:		28-4-2025
CR: 	    Yechiel Shirion
*/

#include "ThreadPool.hpp"

using namespace std;

namespace ilrd_166_7
{
    class JoinThread : public ThreadPool::ITask
    {
    public:
        JoinThread(thread::id id, map<thread::id, shared_ptr<thread>> &threads, mutex &mapLock)
            : m_id(id), m_threads(threads), m_mapLock(mapLock)
        {
            // empty block
        }

        void Run() override
        {
            std::shared_ptr<std::thread> thread;

            {
                unique_lock<mutex> lock(m_mapLock);

                auto iter = m_threads.find(m_id);
                if (iter != m_threads.end())
                {
                    thread = iter->second;
                    m_threads.erase(iter);
                }
            }

            if (thread && thread->joinable())
            {
                thread->join();
            }
        }

    private:
        thread::id m_id;
        map<thread::id, shared_ptr<thread>> &m_threads;
        mutex &m_mapLock;
    };

    static const ThreadPool::Priority POISEN_APPLE = static_cast<ThreadPool::Priority>(4);
    static const ThreadPool::Priority JOIN = static_cast<ThreadPool::Priority>(5);

    ThreadPool::ThreadPool(size_t numOfThreads) : m_running(false),
                                                  m_numOfThreads(0)
    {
        AddThreads(numOfThreads);
    }

    ThreadPool::~ThreadPool() noexcept
    {
        SetNumOfThreads(0);

        for (auto &pair : m_threads)
        {
            if (pair.second->joinable())
            {
                pair.second->join();
            }
        }
    }

    void ThreadPool::SetNumOfThreads(size_t numOfThreads)
    {
        if (numOfThreads >= m_numOfThreads)
        {
            AddThreads(numOfThreads - m_numOfThreads);
        }
        else
        {
            RemoveThreads(m_numOfThreads - numOfThreads);
        }
    }

    void ThreadPool::AddTask(std::shared_ptr<ITask> task, Priority priority)
    {
        lock_guard<std::mutex> lock(m_cvLock);
        m_taskQueue.Push(pair_t(task, priority));
        m_cv.notify_one();
    }

    void ThreadPool::AddTask(std::function<void()> func, Priority priority)
    {
        shared_ptr<ITask> funcP(new FunctionTask(func));

        AddTask(funcP, priority);
    }

    void ThreadPool::Run() noexcept
    {
        lock_guard<std::mutex> lock(m_cvLock);
        m_running = true;
        m_cv.notify_all();
    }

    void ThreadPool::Pause() noexcept
    {
        lock_guard<std::mutex> lock(m_cvLock);
        m_running = false;
    }

    void ThreadPool::ThreadLifeCycle()
    {
        while (true)
        {
            pair_t currTask;
            {
                unique_lock<mutex> lock(m_cvLock);

                m_cv.wait(lock, [this]
                          { return m_running && !m_taskQueue.IsEmpty(); });

                lock.unlock();

                m_taskQueue.Pop(currTask);
            }

            if (currTask.second != POISEN_APPLE)
            {
                currTask.first->Run();
            }
            else
            {
                break;
            }
        }
        auto threadId = this_thread::get_id();
        shared_ptr<ITask> joinTask(new JoinThread(threadId, m_threads, m_mapLock));
        AddTask(move(joinTask), JOIN);
    }

    void ThreadPool::AddThreads(size_t num)
    {
        for (size_t i = 0; i < num; ++i)
        {
            auto newThread = make_shared<thread>(&ThreadPool::ThreadLifeCycle, this);
            lock_guard<std::mutex> lock(m_cvLock);
            m_threads[newThread->get_id()] = newThread;
        }

        m_numOfThreads += num;
    }

    void ThreadPool::RemoveThreads(size_t num)
    {
        for (size_t i = 0; i < num; ++i)
        {
            shared_ptr<ITask> funcP(new FunctionTask(nullptr));
            AddTask(funcP, POISEN_APPLE);
        }

        m_numOfThreads -= num;
    }

    ilrd_166_7::FunctionTask::FunctionTask(std::function<void()> func) : m_func(func)
    {
        // empty
    }

    ilrd_166_7::FunctionTask::~FunctionTask()
    {
        // empty
    }

    void ilrd_166_7::FunctionTask::Run()
    {
        if (m_func)
        {
            m_func();
        }
    }
} // end namespace ilrd__166_7
