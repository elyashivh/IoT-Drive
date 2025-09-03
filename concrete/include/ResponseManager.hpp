#pragma once

#include <unordered_map>
#include <arpa/inet.h>
#include <mutex>

#include "UID.hpp"
#include "Reactor.hpp"
#include "NBDProxy.hpp"
#include "ResponseMessage.hpp"

namespace ilrd_166_7
{
    class ResponseManager
    {
    public:
        explicit ResponseManager();
        ~ResponseManager();

        void RegisterUID(const UID &command_uid, const char *nbd_uid);
        bool IsUIDExists(const UID &command_uid);

    private:
        std::mutex m_lock;
        std::unordered_map<UID, std::array<char, 8>> m_command_uid_map;

        struct sockaddr_in m_master_add{};
        int m_udp;

        LinuxListener m_listener;
        Reactor m_reactor;
        std::thread m_thread;

        std::shared_ptr<AbstractResponseMessage> GetMessageObj();

        void GotResponse(int, Mode);
    }; // ResponseManager
} // ilrd_166_7
