#pragma once

#include <cstddef>

#include <UID.hpp>

namespace ilrd_166_7
{

    class IMinionProxy
    {
    public:
        virtual ~IMinionProxy() = default;

        virtual void Read(const UID &uid, size_t offset, size_t len) = 0;
        virtual void Write(const UID &uid, size_t offset, size_t len, const char *buffer) = 0;
    }; //IMinionProxy

} // namespace ilrd_166_7
