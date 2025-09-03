#include <unistd.h>
#include <iostream>

#include "UID.hpp"

using namespace std; 

namespace ilrd_166_7
{
    std::atomic_uint32_t UID::s_counter{0};

    UID::UID() : m_pid(getpid()), m_timestamp(time(NULL)), m_counter(s_counter++) {}

    bool UID::operator==(const UID &other) const
    {
        return (other.m_pid == m_pid) && (other.m_timestamp == m_timestamp) && (other.m_counter == m_counter);
    }

    bool UID::operator!=(const UID &other) const
    {
        return !(*this == other);
    }

    char *UID::ToBuffer(char *buffer)
    {
        *(pid_t *)buffer = m_pid;
        buffer += sizeof(pid_t);

        *(time_t *)buffer = m_timestamp;
        buffer += sizeof(time_t);

        *(uint32_t *)buffer = m_counter;
        buffer += sizeof(uint32_t);

        return buffer;
    }

    char *UID::FromBuffer(char *buffer)
    {
        m_pid = *(pid_t *)buffer;
        buffer += sizeof(pid_t);

        m_timestamp = *(time_t *)buffer;
        buffer += sizeof(time_t);

        m_counter = *(uint32_t *)buffer;
        buffer += sizeof(uint32_t);

        return buffer;
    }

    uint32_t UID::GetSize() const
    {
        return sizeof(m_pid) + sizeof(m_timestamp) + sizeof(m_counter);
    }

    pid_t UID::GetPid() const
    {
        return m_pid;
    }

    time_t UID::GetTimestamp() const
    {
        return m_timestamp;
    }

    uint32_t UID::GetCounter() const
    {
        return m_counter;
    }

    std::ostream &operator<<(std::ostream &os, const UID &src)
    {
        os << src.GetPid() << " | " << src.GetTimestamp() << " | " << src.GetCounter();

        return os;
    }
}
