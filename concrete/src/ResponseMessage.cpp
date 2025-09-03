#include <cstring>
#include <stdexcept>

#include "ResponseMessage.hpp"

namespace ilrd_166_7
{
    AbstractResponseMessage::AbstractResponseMessage() {}

    AbstractResponseMessage::AbstractResponseMessage(MSG_TYPE key, const UID &uid, uint8_t status)
        : m_key(key), m_uid(uid), m_status(status) {}

    char *AbstractResponseMessage::ToBuffer(char *buffer)
    {
        *(MSG_TYPE *)buffer = m_key;
        buffer += sizeof(m_key);

        buffer = m_uid.ToBuffer(buffer);

        *(uint8_t *)buffer = m_status;
        buffer += sizeof(m_status);

        return buffer;
    }

    char *AbstractResponseMessage::FromBuffer(char *buffer)
    {
        m_key = *(MSG_TYPE *)buffer;
        buffer += sizeof(m_key);

        buffer = m_uid.FromBuffer(buffer);

        m_status = *(uint8_t *)buffer;
        buffer += sizeof(m_status);

        return buffer;
    }

    uint32_t AbstractResponseMessage::GetMessageSize() const
    {
        return sizeof(uint32_t) + m_uid.GetSize() + sizeof(uint8_t);
    }

    AbstractResponseMessage::MSG_TYPE AbstractResponseMessage::GetKey() const
    {
        return m_key;
    }

    UID AbstractResponseMessage::GetUID()
    {
        return m_uid;
    }

    uint8_t AbstractResponseMessage::GetStatus() const
    {
        return m_status;
    }

    ResponseReadMessage::ResponseReadMessage() : m_buffer(nullptr) {}

    ResponseReadMessage::ResponseReadMessage(
        const UID &uid,
        uint8_t status,
        uint32_t size,
        const char *buffer
    )
        : AbstractResponseMessage(MSG_TYPE::READ, uid, status), m_size(size)
    {
        m_buffer = new char[size];

        memcpy(m_buffer, buffer, size);
    }

    ResponseReadMessage::~ResponseReadMessage()
    {
        delete[] m_buffer;
    }

    char *ResponseReadMessage::ToBuffer(char *buffer)
    {
        *(uint32_t *)buffer = GetMessageSize();
        buffer += sizeof(uint32_t);

        buffer = AbstractResponseMessage::ToBuffer(buffer);

        *(uint32_t *)buffer = m_size;
        buffer += sizeof(m_size);

        memcpy(buffer, m_buffer, m_size);

        return buffer;
    }

    char *ResponseReadMessage::FromBuffer(char *buffer)
    {
        buffer += sizeof(uint32_t);

        buffer = AbstractResponseMessage::FromBuffer(buffer);

        m_size = *(uint32_t *)buffer;
        buffer += sizeof(m_size);

        if (m_buffer != nullptr)
        {
            throw std::invalid_argument("buffer is already initialized");
        }

        m_buffer = new char[m_size];

        memcpy(m_buffer, buffer, m_size);

        return buffer + m_size;
    }

    uint32_t ResponseReadMessage::GetMessageSize() const
    {
        return
            sizeof(uint32_t) +
            AbstractResponseMessage::GetMessageSize() +
            sizeof(uint8_t) +
            sizeof(m_size) +
            m_size;
    }

    uint32_t ResponseReadMessage::GetSize() const
    {
        return m_size;
    }

    char *ResponseReadMessage::GetBuffer() const
    {
        return m_buffer;
    }

    ResponseWriteMessage::ResponseWriteMessage() {}

    ResponseWriteMessage::ResponseWriteMessage(const UID &uid, uint8_t status)
        : AbstractResponseMessage(MSG_TYPE::WRITE, uid, status) {}

    char *ResponseWriteMessage::ToBuffer(char *buffer)
    {
        *(uint32_t *)buffer = GetMessageSize();
        buffer += sizeof(uint32_t);

        buffer = AbstractResponseMessage::ToBuffer(buffer);

        return buffer;
    }

    char *ResponseWriteMessage::FromBuffer(char *buffer)
    {
        buffer += sizeof(uint32_t);

        buffer = AbstractResponseMessage::FromBuffer(buffer);

        return buffer;
    }

    uint32_t ResponseWriteMessage::GetMessageSize() const
    {
        return sizeof(uint32_t) + AbstractResponseMessage::GetMessageSize() + sizeof(uint8_t);
    }

    uint32_t ResponseWriteMessage::GetSize() const
    {
        return m_size;
    }
}
