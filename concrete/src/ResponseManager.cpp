#include <cstring>
#include <iostream>
#include <array>

#include "NetworkUtils.h"
#include "Handleton.hpp"
#include "Factory.hpp"
#include "NBDProxy.hpp"
#include "ResponseManager.hpp"

using namespace std;
namespace ilrd_166_7
{
    static const int PORT = 8080;

    ResponseManager::ResponseManager() : m_reactor(&m_listener)
    {
        m_udp = SocketCreate(SOCK_DGRAM, &m_master_add, PORT);
        SocketBind(m_udp, &m_master_add);

        m_reactor.Register(
            m_udp,
            Mode::READ,
            [this](int fd, Mode mode) { GotResponse(fd, mode); }
        );

        m_thread = std::thread([this]() { m_reactor.Run(); });
    }

    ResponseManager::~ResponseManager()
    {
        SocketClose(m_udp);

        m_reactor.Stop();

        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

    void ResponseManager::RegisterUID(const UID &command_uid, const char *nbd_uid)
    {
        unique_lock<std::mutex> lock(m_lock);
        array<char, 8> handle_array;
        memcpy(handle_array.data(), nbd_uid, 8);
        m_command_uid_map.insert({command_uid, handle_array});
    }

    bool ResponseManager::IsUIDExists(const UID &command_uid)
    {
        unique_lock<std::mutex> lock(m_lock);

        auto it = m_command_uid_map.find(command_uid);
        if (it == m_command_uid_map.end())
        {
            return false;
        }

        return true;
    }

    void ResponseManager::GotResponse(int fd, Mode mode)
    {
        (void)mode;
        (void)fd;

        try
        {
            auto msg = GetMessageObj();

            auto nbd_proxy = Handleton<NBDProxy>::GetInstance();

            {
                lock_guard<std::mutex> lock(m_lock);
                auto nbd_uid = m_command_uid_map.at(msg->GetUID()).data();

                if (msg->GetKey() == AbstractResponseMessage::MSG_TYPE::READ)
                {
                    nbd_proxy->ReadResponse(
                        nbd_uid,
                        msg->GetSize(),
                        dynamic_pointer_cast<ResponseReadMessage>(msg)->GetBuffer()
                    );
                }

                m_command_uid_map.erase(msg->GetUID());
            }
        }

        catch (const std::exception &e)
        {
            std::cerr << "GotResponse - Exception Message: " << e.what() << std::endl;
        }
    }

    std::shared_ptr<AbstractResponseMessage> ResponseManager::GetMessageObj()
    {
        uint32_t size = 0;
        UDPReceiveFrom(m_udp, (char *)&size, sizeof(uint32_t), MSG_PEEK, nullptr);

        std::vector<char> buffer(size);
        UDPReceiveFrom(m_udp, buffer.data(), size, 0, nullptr);

        auto msg_key = *(AbstractResponseMessage::MSG_TYPE *)(buffer.data() + sizeof(uint32_t));
        auto msg = Handleton<Factory<AbstractResponseMessage, AbstractResponseMessage::MSG_TYPE>>
            ::GetInstance()->Create(msg_key);

        msg->FromBuffer(buffer.data());

        return msg;
    }
}
