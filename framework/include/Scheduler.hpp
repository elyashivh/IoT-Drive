#pragma once

#include <memory> //shared ptr
#include <chrono>

namespace ilrd_166_7
{

    /**
     * @class Scheduler
     * @brief Basic scheduling interface for executing tasks after a delay.
     *
     * The Scheduler class provides an interface for adding delayed tasks
     * (instances of ISchedulerTask) to be executed asynchronously after
     * a specified timeout.
     */
    class Scheduler
    {
    public:
        /**
         * @class ISchedulerTask
         * @brief Interface for user-defined tasks to be scheduled.
         *
         * Any task to be scheduled must inherit from this interface
         * and implement the Execute method.
         */
        class ISchedulerTask
        {
        public:
            /**
             * @brief Task logic to be executed when the scheduled time is reached.
             */
            virtual void Execute() = 0;
        };

        /**
         * @brief Adds a task to be executed after a given delay.
         *
         * @param task A shared pointer to an object implementing ISchedulerTask.
         * @param ms Delay in milliseconds before the task is executed.
         *
         * @note Task ownership is managed via shared_ptr. The actual implementation
         * of scheduling (e.g., via threads, timers, etc.) must be handled by the
         * class that implements this function.
         */
        void AddTask(std::shared_ptr<ISchedulerTask> task, std::chrono::milliseconds ms);

    }; // end Scheduler
} // end namespace ilrd_166_7