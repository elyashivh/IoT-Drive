#pragma once

#include <arpa/inet.h>

#include "Message.hpp"
#include "ResponseMessage.hpp"
#include "FileProxy.hpp"
#include "Framework.hpp"

namespace ilrd_166_7
{
    class MasterProxy : public IInputProxy
    {
    public:
        explicit MasterProxy();
        ~MasterProxy();

        std::shared_ptr<IKeyArgs> GetKeyArgs(fdAndMode key) override;

        int GetUDP() const;
        FileProxy &GetFile();

        void SendResponse(std::shared_ptr<AbstractResponseMessage> msg);

    private:
        struct sockaddr_in m_minion_add{};
        struct sockaddr_in m_master_add{};
        int m_udp_rec;
        int m_udp_send;

        FileProxy m_file;

        std::shared_ptr<AbstractMessage> GetMessageObj();
    }; // MasterProxy
} // ilrd_166_7
