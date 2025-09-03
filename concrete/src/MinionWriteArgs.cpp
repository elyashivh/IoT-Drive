
#include <iostream>
#include <cstring>

#include "MinionWriteArgs.hpp"

namespace ilrd_166_7
{
    MinionWriteArgs::MinionWriteArgs(UID uid, u_int32_t offset, u_int32_t size, const char *buffer)
        : m_uid(uid), m_offset(offset), m_size(size)
    {
        m_buffer = new char[size];

        if (buffer == nullptr)
        {
            throw std::invalid_argument("Received null pointer");
        }

        memcpy(m_buffer, buffer, size);
    }

    MinionWriteArgs::~MinionWriteArgs()
    {
        delete[] m_buffer;
    }

    void MinionWriteArgs::SetUid(UID uid)
    {
        m_uid = uid;
    }

    void MinionWriteArgs::SetOffset(u_int32_t offset)
    {
        m_offset = offset;
    }

    void MinionWriteArgs::SetSize(u_int32_t size)
    {
        m_size = size;
    }

    void MinionWriteArgs::SetBuffer(const char *buffer)
    {
        memcpy(m_buffer, buffer, m_size);
    }

    int MinionWriteArgs::GetKey()
    {
        return static_cast<int>(Mode::WRITE);
    }

    UID MinionWriteArgs::GetUid() const
    {
        return m_uid;
    }

    u_int32_t MinionWriteArgs::GetOffset() const
    {
        return m_offset;
    }

    u_int32_t MinionWriteArgs::GetSize() const
    {
        return m_size;
    }

    const char *MinionWriteArgs::GetBuffer() const
    {
        return m_buffer;
    }
}
