#include <iostream>

#include "NetworkUtils.h"
#include "Handleton.hpp"
#include "Factory.hpp"
#include "MinionReadArgs.hpp"
#include "MinionWriteArgs.hpp"
#include "MasterProxy.hpp"

namespace ilrd_166_7
{
    static const int PORT_SEND = 8080;

#ifndef PORT_REC
    #define PORT_REC 8000
#endif

#ifndef FILE_PATH
    #define FILE_PATH "./minions/minion_1.txt"
#endif

    MasterProxy::MasterProxy() : m_file(FILE_PATH)
    {
        m_udp_rec = SocketCreate(SOCK_DGRAM, &m_minion_add, PORT_REC);
        SocketBind(m_udp_rec, &m_minion_add);

        m_udp_send = SocketCreate(SOCK_DGRAM, &m_master_add, PORT_SEND);
        ConvertIP(m_udp_send, "127.0.0.1", &m_master_add);
    }

    MasterProxy::~MasterProxy()
    {
        SocketClose(m_udp_rec);
        SocketClose(m_udp_send);
    }

    std::shared_ptr<IKeyArgs> MasterProxy::GetKeyArgs(fdAndMode key)
    {
        (void)key;

        auto msg = GetMessageObj();

        if (msg->GetKey() == AbstractMessage::MSG_TYPE::READ)
        {
            return std::make_shared<MinionReadArgs>(
                msg->GetUID(),
                msg->GetOffset(),
                msg->GetSize()
            );
        }

        return std::make_shared<MinionWriteArgs>(
            msg->GetUID(),
            msg->GetOffset(),
            msg->GetSize(),
            std::dynamic_pointer_cast<WriteMessage>(msg)->GetBuffer()
        );
    }

    int MasterProxy::GetUDP() const
    {
        return m_udp_rec;
    }

    FileProxy &MasterProxy::GetFile()
    {
        return m_file;
    }

    void MasterProxy::SendResponse(std::shared_ptr<AbstractResponseMessage> msg)
    {
        uint32_t size = msg->GetMessageSize();

        char *buffer = new char[size];

        msg->ToBuffer(buffer);

        UDPSendTo(m_udp_send, buffer, size, &m_master_add);

        delete[] buffer;
    }

    std::shared_ptr<AbstractMessage> MasterProxy::GetMessageObj()
    {
        uint32_t size = 0;
        UDPReceiveFrom(m_udp_rec, (char *)&size, sizeof(uint32_t), MSG_PEEK, nullptr);

        char *buffer = new char[size];
        UDPReceiveFrom(m_udp_rec, buffer, size, 0, nullptr);

        auto msg_key = *(AbstractMessage::MSG_TYPE *)(buffer + sizeof(uint32_t));
        auto msg = Handleton<Factory<AbstractMessage, AbstractMessage::MSG_TYPE>>
            ::GetInstance()->Create(msg_key);

        msg->FromBuffer(buffer);

        delete[] buffer;

        return msg;
    }
}
