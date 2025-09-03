#pragma once

#include "UID.hpp"
#include "Framework.hpp"

namespace ilrd_166_7
{
    class MinionWriteArgs : public IKeyArgs
    {
    public:
        MinionWriteArgs(
            UID m_uid,
            u_int32_t offset = 0,
            u_int32_t size = 0,
            const char *buffer = nullptr);
        ~MinionWriteArgs();

        int GetKey() override;
        UID GetUid() const;
        u_int32_t GetOffset() const;
        u_int32_t GetSize() const;
        const char *GetBuffer() const;

        void SetUid(UID uid);
        void SetOffset(u_int32_t offset);
        void SetSize(u_int32_t size);
        void SetBuffer(const char *buffer);

    private:
        UID m_uid;
        u_int32_t m_offset;
        u_int32_t m_size;
        char *m_buffer;
    }; // MinionWriteArgs
} // ilrd_166_7

