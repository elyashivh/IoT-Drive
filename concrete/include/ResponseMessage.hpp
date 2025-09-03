#pragma once

#include <cstdint>

#include "UID.hpp"

namespace ilrd_166_7
{
    class AbstractResponseMessage
    {
    public:
        enum MSG_TYPE : uint32_t
        {
            READ,
            WRITE
        };

        explicit AbstractResponseMessage();
        explicit AbstractResponseMessage(MSG_TYPE key, const UID &uid, uint8_t status);
        virtual ~AbstractResponseMessage() = default;

        virtual char *ToBuffer(char *buffer) = 0;
        virtual char *FromBuffer(char *buffer) = 0;

        virtual uint32_t GetMessageSize() const = 0;
        virtual uint32_t GetSize() const = 0;
        MSG_TYPE GetKey() const;
        UID GetUID();
        uint8_t GetStatus() const;

    private:
        MSG_TYPE m_key;
        UID m_uid;
        uint8_t m_status;
    }; // AbstractResponseMessage

    class ResponseReadMessage : public AbstractResponseMessage
    {
    public:
        explicit ResponseReadMessage();
        explicit ResponseReadMessage(
            const UID &uid,
            uint8_t status,
            uint32_t size,
            const char *buffer);
        ~ResponseReadMessage();

        char *ToBuffer(char *buffer) override;
        char *FromBuffer(char *buffer) override;

        uint32_t GetMessageSize() const override;
        uint32_t GetSize() const override;
        char *GetBuffer() const;

    private:
        uint32_t m_size;
        char *m_buffer;
    }; // ResponseReadMessage

    class ResponseWriteMessage : public AbstractResponseMessage
    {
    public:
        explicit ResponseWriteMessage();
        explicit ResponseWriteMessage(const UID &uid, uint8_t status);

        char *ToBuffer(char *buffer) override;
        char *FromBuffer(char *buffer) override;

        uint32_t GetMessageSize() const override;
        uint32_t GetSize() const override;

    private:
        uint32_t m_size;
    }; // ResponseWriteMessage
} // ilrd_166_7
