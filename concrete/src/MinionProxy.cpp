#include "NetworkUtils.h"
#include "MinionProxy.hpp"

namespace ilrd_166_7
{
    MinionProxy::MinionProxy(u_int16_t port, const char *ip)
    {
        m_udp = SocketCreate(SOCK_DGRAM, &m_minion_add, port);

        ConvertIP(m_udp, ip, &m_minion_add);
    }

    MinionProxy::~MinionProxy()
    {
        SocketClose(m_udp);
    }

    void MinionProxy::Read(const UID &uid, size_t offset, size_t size)
    {
        ReadMessage msg_obj(uid, offset, size);

        SerializeSend(msg_obj);
    }

    void MinionProxy::Write(const UID &uid, size_t offset, size_t size, const char *buffer)
    {
        WriteMessage msg_obj(uid, offset, size, buffer);
        
        SerializeSend(msg_obj);
    }

    void MinionProxy::SerializeSend(AbstractMessage &msg_obj)
    {
        uint32_t size = msg_obj.GetMessageSize();

        char *buffer = new char[size];

        msg_obj.ToBuffer(buffer);

        UDPSendTo(m_udp, buffer, size, &m_minion_add);

        delete[] buffer;
    }
} // namespace ilrd_166_7
