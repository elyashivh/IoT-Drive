#pragma once

#include <arpa/inet.h> // socket
#include <cstddef>

#include "IMinionProxy.hpp"
#include "Message.hpp"

namespace ilrd_166_7
{

    class MinionProxy : public IMinionProxy
    {
    public:
        explicit MinionProxy(u_int16_t port = 8080, const char *ip = "127.0.0.1");
        ~MinionProxy();

        void Read(const UID &uid, size_t offset, size_t size) override;
        void Write(const UID &uid, size_t offset, size_t size, const char *buffer) override;

    private:
        struct sockaddr_in m_minion_add{};
        int m_udp;

        void SerializeSend(AbstractMessage &msg_obj);
    }; // MinionProxy

} // namespace ilrd_166_7
