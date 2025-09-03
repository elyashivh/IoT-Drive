#include <cstring>
#include <stdexcept>

#include "Message.hpp"

namespace ilrd_166_7
{
    AbstractMessage::AbstractMessage(AbstractMessage::MSG_TYPE key, const UID &uid)
        : m_key(key), m_uid(uid) {}

    char *AbstractMessage::ToBuffer(char *buffer)
    {
        *(MSG_TYPE *)buffer = m_key;
        buffer += sizeof(m_key);

        buffer = m_uid.ToBuffer(buffer);

        return buffer;
    }

    char *AbstractMessage::FromBuffer(char *buffer)
    {
        m_key = *(MSG_TYPE *)buffer;
        buffer += sizeof(m_key);

        buffer = m_uid.FromBuffer(buffer);

        return buffer;
    }

    AbstractMessage::MSG_TYPE AbstractMessage::GetKey()
    {
        return m_key;
    }

    uint32_t AbstractMessage::GetMessageSize()
    {
        return sizeof(uint32_t) + m_uid.GetSize();
    }

    UID AbstractMessage::GetUID()
    {
        return m_uid;
    }

    ReadMessage::ReadMessage() {}

    ReadMessage::ReadMessage(const UID &uid, uint32_t offset, uint32_t size)
        : AbstractMessage(AbstractMessage::READ, uid), m_offset(offset), m_size(size) {}

    char *ReadMessage::ToBuffer(char *buffer)
    {
        *(uint32_t *)buffer = GetMessageSize();
        buffer += sizeof(uint32_t);

        buffer = AbstractMessage::ToBuffer(buffer);

        *(uint32_t *)buffer = m_offset;
        buffer += sizeof(m_offset);

        *(uint32_t *)buffer = m_size;
        buffer += sizeof(m_size);

        return buffer;
    }

    char *ReadMessage::FromBuffer(char *buffer)
    {
        buffer += sizeof(uint32_t);

        buffer = AbstractMessage::FromBuffer(buffer);

        m_offset = *(uint32_t *)buffer;
        buffer += sizeof(m_offset);

        m_size = *(uint32_t *)buffer;
        buffer += sizeof(m_size);

        return buffer;
    }

    uint32_t ReadMessage::GetMessageSize()
    {
        return
            sizeof(uint32_t)
            + AbstractMessage::GetMessageSize()
            + sizeof(m_offset)
            + sizeof(m_size);
    }

    uint32_t ReadMessage::GetOffset()
    {
        return m_offset;
    }

    uint32_t ReadMessage::GetSize()
    {
        return m_size;
    }

    WriteMessage::WriteMessage() {}

    WriteMessage::WriteMessage(const UID &uid, uint32_t offset, uint32_t size, const char *buffer)
        : AbstractMessage(AbstractMessage::WRITE, uid), m_offset(offset), m_size(size)
    {
        m_buffer = new char[size];

        if (buffer == nullptr)
        {
            throw std::invalid_argument("Received nullptr");
        }

        memcpy(m_buffer, buffer, size);
    }

    WriteMessage::~WriteMessage()
    {
        delete[] m_buffer;
    }

    char *WriteMessage::ToBuffer(char *buffer)
    {
        *(uint32_t *)buffer = GetMessageSize();
        buffer += sizeof(uint32_t);

        buffer = AbstractMessage::ToBuffer(buffer);

        *(uint32_t *)buffer = m_offset;
        buffer += sizeof(m_offset);

        *(uint32_t *)buffer = m_size;
        buffer += sizeof(m_size);

        memcpy(buffer, m_buffer, m_size);

        return buffer + m_size;
    }

    char *WriteMessage::FromBuffer(char *buffer)
    {
        buffer += sizeof(uint32_t);

        buffer = AbstractMessage::FromBuffer(buffer);

        m_offset = *(uint32_t *)buffer;
        buffer += sizeof(m_offset);

        m_size = *(uint32_t *)buffer;
        buffer += sizeof(m_size);

        m_buffer = new char[m_size];

        memcpy(m_buffer, buffer, m_size);

        return buffer + m_size;
    }

    uint32_t WriteMessage::GetMessageSize()
    {
        return
            sizeof(uint32_t)
            + AbstractMessage::GetMessageSize()
            + sizeof(m_offset)
            + sizeof(m_size)
            + m_size;
    }

    uint32_t WriteMessage::GetOffset()
    {
        return m_offset;
    }

    uint32_t WriteMessage::GetSize()
    {
        return m_size;
    }

    char *WriteMessage::GetBuffer()
    {
        return m_buffer;
    }
}
