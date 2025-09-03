#pragma once

#include <atomic>
#include <time.h>
#include <sys/types.h>
#include <functional>
#include <iosfwd>

namespace ilrd_166_7
{
    class UID
    {
    public:
        explicit UID();
        ~UID() = default;

        UID(const UID &other) = default;
        UID(UID &&other) = default;

        UID &operator=(const UID &other) = default;
        UID &operator=(UID &&other) = default;
        bool operator==(const UID &other) const;
        bool operator!=(const UID &other) const;

        char *ToBuffer(char *buffer);
        char *FromBuffer(char *buffer);
        uint32_t GetSize() const;

        pid_t GetPid() const;
        time_t GetTimestamp() const;
        uint32_t GetCounter() const;

    private:
        pid_t m_pid;
        time_t m_timestamp;
        uint32_t m_counter;

        static std::atomic_uint32_t s_counter;
    }; // Uid

    std::ostream &operator<<(std::ostream &os, const UID &src);
} // ilrd_166_7

namespace std
{
    template <>
    struct hash<ilrd_166_7::UID>
    {
        std::size_t operator()(const ilrd_166_7::UID &uid) const
        {
            std::size_t h1 = std::hash<pid_t>()(uid.GetPid());
            std::size_t h2 = std::hash<time_t>()(uid.GetTimestamp());
            std::size_t h3 = std::hash<uint32_t>()(uid.GetCounter());

            return (h1 ^ (h2 << 1) ^ (h3 << 2));
        }
    }; // hash<ilrd_166_7::Uid>
} // std

