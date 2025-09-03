#pragma once

#include <functional>
#include <memory>

#include "Scheduler.hpp"

namespace ilrd_166_7
{
    /**
     * @class AsyncInjection
     * @brief Executes a user-defined function asynchronously at fixed time intervals.

     * The AsyncInjection class allows periodic asynchronous execution of a callable
     * (std::function<bool()>) through an external Scheduler. The function is repeatedly
     * executed every specified interval (in milliseconds), until it returns false.
     */
    class AsyncInjection
    {
    public:
        /**
         * @brief Constructs an AsyncInjection object.
         *
         * @param func A function to execute periodically. Should return true to continue scheduling, false to stop.
         * @param ms The interval in milliseconds between executions.
         */
        AsyncInjection(std::function<bool()> func, std::chrono::milliseconds ms);
        AsyncInjection(const AsyncInjection &other) = delete;
        AsyncInjection &operator=(const AsyncInjection &other) = delete;

        /**
         * @brief Starts the asynchronous execution by scheduling the first task.
         */
        void PerformAction();

    private:
        class AsyncInjectionTask : public Scheduler::ISchedulerTask
        {
        public:
            AsyncInjectionTask(AsyncInjection &asyncInjection);

            void Execute() override;

        private:
            AsyncInjection &m_asyncInjection;
        }; // end AsyncInjectionTask

        ~AsyncInjection() = default;
        std::function<bool()> m_func;
        std::chrono::milliseconds m_interval;
        std::shared_ptr<AsyncInjectionTask> m_task;

    }; // class AsyncInjectionTask
} // end namespace ilrd_166_7