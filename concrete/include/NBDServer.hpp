#pragma once

#include <memory>

#include "Framework.hpp"

namespace ilrd_166_7
{

    /**
     * @class  NBDServer
     * @brief  Manages NBD device communication and handles NBD requests
     */

    class NBDServer
    {
    public:
        NBDServer();
        ~NBDServer();

        int GetFD();
        std::shared_ptr<IKeyArgs> GetKeyArgs();
        void Respond(char handle[8], uint32_t size, const char *buffer);
        void Respond(char handle[8], uint8_t status);

    private:
        int SendStatus(char handle[8], uint32_t status);
        int m_sp[2];

    }; // NBDServer
} // namespace ilrd_166_7
