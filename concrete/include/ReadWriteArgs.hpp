#pragma once

#include <vector>

#include "Framework.hpp"

namespace ilrd_166_7
{

enum Key
{
    READ,
    WRITE
};

class ReadArgs : public IKeyArgs
{
public:
    ReadArgs(size_t length, size_t offset);
    int GetKey();
    size_t GetLength();
    size_t GetOffset();

private:
    size_t m_length;
    size_t m_offset;
};   // end ReadArgs  


class WriteArgs : public IKeyArgs
{
public: 
    WriteArgs(size_t length, size_t offset, const char* buffer);
    ~WriteArgs();
    int GetKey();
    size_t GetLength();
    size_t GetOffset();
    char* GetBuffer();
    void SetBuffer(int fd);

private:
    size_t m_length;
    size_t m_offset;
    char* m_buffer;

};       // end ReadArgs  
}       // end namespace  
