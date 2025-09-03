#pragma once

#include "UID.hpp"
#include "Framework.hpp"

namespace ilrd_166_7
{
    class MinionReadArgs : public IKeyArgs
    {
    public:
        explicit MinionReadArgs(UID uid, u_int32_t offset = 0, u_int32_t size = 0);

        int GetKey() override;
        UID GetUid() const;
        u_int32_t GetOffset() const;
        u_int32_t GetSize() const;

        void SetUid(UID uid);
        void SetOffset(u_int32_t offset);
        void SetSize(u_int32_t size);

    private:
        UID m_uid;
        u_int32_t m_offset;
        u_int32_t m_size;
    }; // MinionReadArgs
} // ilrd_166_7
