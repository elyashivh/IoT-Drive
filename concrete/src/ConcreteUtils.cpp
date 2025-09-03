#include <stdexcept> //runtime error

#include "ConcreteUtils.hpp"

namespace ilrd_166_7
{

    void ErrorValidation(int status, const std::string& msg)
    {
        if (status < 0)
        {
            //SendLog(msg);
            throw std::runtime_error(msg);
        }
    }
    
    void SendLog(const std::string& msg)
    {
        Handleton<Logger>::GetInstance()->Log(msg, Logger::SEVERITY::ERROR);
    }

    uint32_t ExtractKey(const char* buffer)
    {
        return(*(uint32_t*)(buffer + sizeof(uint32_t)));
    }


} // end namespace ilrd_166_7
