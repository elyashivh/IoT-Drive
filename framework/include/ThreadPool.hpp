#ifndef __ILRD_THREADPOOL_HPP__
#define __ILRD_THREADPOOL_HPP__

#include <condition_variable> //condition variable
#include <mutex>              //mutex
#include <memory>             //for shared_ptr
#include <thread>             //thread
#include <functional>         //function
#include <atomic>             //atomic vars
#include <map>                //map container

#include "WaitableQueue.hpp"
#include "PQueue.hpp"

namespace ilrd_166_7
{

    /**
     * @class ThreadPool
     * @brief A multi-threaded task execution engine with priority-based scheduling.
     *
     * The ThreadPool manages a pool of threads that process submitted tasks based on their priority.
     * It supports dynamic thread scaling, task prioritization, and safe concurrent task submission.
     *
     * Tasks must inherit from ITask and implement the Run() method.
     */
    class ThreadPool
    {
    public:
        /**
         * @enum Priority
         * @brief Defines the priority levels for task execution.
         */
        enum class Priority
        {
            LOW,
            MEDIUM,
            HIGH,
            TOP
        };

        /**
         * @class ITask
         * @brief Abstract interface for tasks to be executed in the thread pool.
         */
        class ITask
        {
        public:
            /**
             * @brief Execute the task.
             */
            void virtual Run() = 0;
            virtual ~ITask() = default;
        };

        explicit ThreadPool(size_t numOfThreads = std::thread::hardware_concurrency());
        ~ThreadPool() noexcept;
        ThreadPool(const ThreadPool &other) = delete;
        ThreadPool &operator=(const ThreadPool &other) = delete;
        void SetNumOfThreads(size_t numOfThreads);
        void AddTask(std::shared_ptr<ITask> task, Priority type); // Task must implement operator<
        void AddTask(std::function<void()> func, Priority type);
        void Run() noexcept;
        void Pause() noexcept;

    private:
        using task_t = std::shared_ptr<ITask>;
        using pair_t = std::pair<task_t, Priority>;
        struct TaskComprator
        {
            bool operator()(const pair_t &var1, const pair_t &var2)
            {
                return var1.second > var2.second;
            }
        };
        using container_t = PQueue<pair_t, std::vector<pair_t>, TaskComprator>;
        WaitableQueue<pair_t, container_t> m_taskQueue;
        std::map<std::thread::id, std::shared_ptr<std::thread>> m_threads;

        std::atomic_bool m_running;
        size_t m_numOfThreads;
        std::condition_variable m_cv;
        std::mutex m_cvLock;
        std::mutex m_mapLock;

        /*Helper Functions*/
        void WaitForTasks();
        void ThreadLifeCycle();
        void AddThreads(size_t num);
        void RemoveThreads(size_t num);
        void CleanUpThreads(void);
    };

    /**
     * @struct FunctionTask
     * @brief A concrete task that wraps a std::function<void()> callable.
     */
    struct FunctionTask : public ThreadPool::ITask
    {
        FunctionTask(std::function<void()> func);
        ~FunctionTask();
        void Run() override;

    private:
        std::function<void()> m_func;
    }; // SimpleTaskS

} // namespace ilrd_166_7

#endif
