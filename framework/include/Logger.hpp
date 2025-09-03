#pragma once

#include <mutex>
#include <thread>
#include <fstream>
#include <atomic>

#include "PQueue.hpp"
#include "WaitableQueue.hpp"
#include "Handleton.hpp"

namespace ilrd_166_7
{

    /**
     * @class Logger
     * @brief Asynchronous logging utility with severity levels.
     *
     * Provides thread-safe, background-processed logging to a file.
     * Logs are pushed into a queue and handled by a dedicated logging thread,
     * ensuring minimal blocking for the calling thread.
     */
    class Logger
    {
    public:
        /**
         * @enum SEVERITY
         * @brief Represents the severity level of a log message.
         */
        enum class SEVERITY
        {
            DEBUG,
            WARNINIG,
            ERROR
        }; // enum class SEVERITY

        explicit Logger();
        ~Logger();
        Logger(const Logger &) = delete;
        Logger &operator=(const Logger &) = delete;

        /**
         * @brief Logs a message with a given severity level.
         *
         * Pushes the message into the internal queue to be handled asynchronously.
         *
         * @param msg The log message to record.
         * @param severity The severity level of the log.
         */
        void Log(const std::string&, SEVERITY);

    private:
        struct LogInfo
        {
            std::string msg;
            SEVERITY sev;
            time_t timestamp;
        }; // struct Info

        WaitableQueue<LogInfo> m_q;
        std::atomic<bool> m_running;
        std::ofstream m_logFile;
        std::thread m_thread;
        std::mutex m_mutex;

        void ThreadJob();
        void WriteToLogger(const LogInfo &);

    }; // class Logger

} // ilrd_166_7
