#include <iostream>

#include "MinionReadArgs.hpp"
#include "RAIDManager.hpp"

namespace ilrd_166_7
{
    MinionReadArgs::MinionReadArgs(UID uid, u_int32_t offset, u_int32_t size)
        : m_uid(uid), m_offset(offset), m_size(size) {}

    void MinionReadArgs::SetUid(UID uid)
    {
        m_uid = uid;
    }

    void MinionReadArgs::SetOffset(u_int32_t offset)
    {
        m_offset = offset;
    }

    void MinionReadArgs::SetSize(u_int32_t size)
    {
        m_size = size;
    }

    int MinionReadArgs::GetKey()
    {
        return static_cast<int>(Mode::READ);
    }

    UID MinionReadArgs::GetUid() const
    {
        return m_uid;
    }

    u_int32_t MinionReadArgs::GetOffset() const
    {
        return m_offset;
    }

    u_int32_t MinionReadArgs::GetSize() const
    {
        return m_size;
    }
}
