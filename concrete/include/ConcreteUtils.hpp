#pragma once  

#include <string>

#include "Logger.hpp"

namespace ilrd_166_7
{

    static const char* ip = "127.0.0.1";

    void ErrorValidation(int status, const std::string& msg);

    void SendLog(const std::string& msg);

    uint32_t ExtractKey(const char* buffer); 
}

