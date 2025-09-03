#include "Logger.hpp"

using namespace std;

namespace ilrd_166_7
{
    /*-------------FORWARD DECLARATION------------------*/
    static string SeverityToString(Logger::SEVERITY sev);

    /*-------------CONST VARS---------------------------*/
    static const string LOGGER_NAME = "~/git/projects/final_project/Log File.txt";

    /*-------------IMPLEMENTAION------------------------*/
    Logger::Logger() :m_running(true)
    {
        m_thread = thread(&Logger::ThreadJob, this);
        m_logFile.open(LOGGER_NAME, fstream::app);
    }

    Logger::~Logger()
    {
        m_running.store(false);
        if (m_thread.joinable())
        {
            m_thread.join();
        }

        m_logFile.close();
    }

    void Logger::Log(const std::string& str, SEVERITY sev)
    {
        lock_guard<mutex> lock(m_mutex);
        LogInfo newLog{str, sev, time(nullptr)};
        try
        {
            m_q.Push(newLog);
        }
        catch(const std::exception& e)
        {
            throw runtime_error("Error: Failed to add new log");
        }
    }

    void Logger::ThreadJob()
    {
        while (m_running.load() || !m_q.IsEmpty())
        {
            LogInfo currLog;

            bool newData = m_q.Pop(currLog, chrono::milliseconds(100));
            if (newData)
            {
                WriteToLogger(currLog);
            }
        }
    }

    void Logger::WriteToLogger(const LogInfo& log)
    {
        char buffer[100];
        strftime(buffer, sizeof(buffer), "%d-%m-%Y - %H:%M:%S", localtime(&log.timestamp));
        m_logFile << buffer << " ||  <" << SeverityToString(log.sev) << "> " << log.msg << endl;
    }

    static string SeverityToString(Logger::SEVERITY sev)
    {
        switch (sev)
        {
        case Logger::SEVERITY::DEBUG:
            return "DEBUG";
            break;
        case Logger::SEVERITY::WARNINIG:
            return "WARNINIG";
            break;
        case Logger::SEVERITY::ERROR:
            return  "ERROR";
            break;

        default:
            return "UNKNOWN";
            break;
        }
    }

}