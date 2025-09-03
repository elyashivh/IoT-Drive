#pragma once

#include <cstdint>

#include "UID.hpp"

namespace ilrd_166_7
{
    class AbstractMessage
    {
    public:
        enum MSG_TYPE : uint32_t
        {
            READ,
            WRITE
        };

        explicit AbstractMessage() = default;
        explicit AbstractMessage(AbstractMessage::MSG_TYPE key, const UID &uid);
        virtual ~AbstractMessage() = default;

        virtual char *ToBuffer(char *buffer) = 0;
        virtual char *FromBuffer(char *buffer) = 0;
        virtual uint32_t GetMessageSize() = 0;

        virtual uint32_t GetOffset() = 0;
        virtual uint32_t GetSize() = 0;
        MSG_TYPE GetKey();
        UID GetUID();

    private:
        MSG_TYPE m_key;
        UID m_uid;
    }; // AbstractMessage

    class ReadMessage : public AbstractMessage
    {
    public:
        explicit ReadMessage();
        explicit ReadMessage(const UID &uid, uint32_t offset = 0, uint32_t size = 0);

        char *ToBuffer(char *buffer) override;
        char *FromBuffer(char *buffer) override;
        uint32_t GetMessageSize() override;

        uint32_t GetOffset() override;
        uint32_t GetSize() override;

    private:
        uint32_t m_offset;
        uint32_t m_size;
    }; // ReadMessage

    class WriteMessage : public AbstractMessage
    {
    public:
        explicit WriteMessage();
        explicit WriteMessage(const UID &uid, uint32_t offset, uint32_t size, const char *buffer);
        ~WriteMessage();

        char *ToBuffer(char *buffer) override;
        char *FromBuffer(char *buffer) override;
        uint32_t GetMessageSize() override;

        uint32_t GetOffset() override;
        uint32_t GetSize() override;
        char *GetBuffer();

    private:
        uint32_t m_offset;
        uint32_t m_size;
        char *m_buffer;
    }; // WriteMessage
} // ilrd_166_7
