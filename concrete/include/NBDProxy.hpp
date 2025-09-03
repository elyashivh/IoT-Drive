#pragma once

#include <memory>

#include "Framework.hpp"
#include "NBDServer.hpp"
#include "ReadWriteArgs.hpp"
#include "Message.hpp"

namespace ilrd_166_7
{
    class NBDProxy : public IInputProxy
    {
    public:
        explicit NBDProxy() = default;
        ~NBDProxy() = default;

        int GetFD();
        std::shared_ptr<IKeyArgs> GetKeyArgs(fdAndMode key) override;

        void ReadResponse(char nbdUid[8], u_int32_t size, const char *buffer);
        void StatusResponse(char nbdUid[8], uint8_t status);

    private:
        NBDServer m_nbd;
    }; // NBDProxy
} // end namespace ilrd_166_7
