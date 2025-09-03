
#include <iostream>
#include <cstring>
#include <unistd.h> //read

#include "ReadWriteArgs.hpp"

using namespace ilrd_166_7;
using namespace std;

namespace ilrd_166_7
{
    /***********READ IMP*************/
    ReadArgs::ReadArgs(size_t length, size_t offset): 
                      m_length(length), m_offset(offset){/*empty*/}

    int ReadArgs::GetKey()
    {
        return READ;
    }

    size_t ReadArgs::GetLength()
    {
        return m_length;
    }

    size_t ReadArgs::GetOffset()
    {
        return m_offset;
    }

    /***********WRITE IMP*************/
    WriteArgs::WriteArgs(size_t length, size_t offset, const char* buffer): 
                      m_length(length), m_offset(offset),
                      m_buffer(new char[length])
    {
        memcpy(m_buffer, buffer, length);
        cout << "here: " << __FILE__ << ". Line:" << __LINE__  << endl; //for tests
    } 

    WriteArgs::~WriteArgs()
    {
        delete[] m_buffer;
    }

    int WriteArgs::GetKey()
    {
        return WRITE;
    }

    size_t WriteArgs::GetLength()
    {
        return m_length;
    }

    size_t WriteArgs::GetOffset()
    {
        return m_offset;
    }

    char* WriteArgs::GetBuffer()
    {
        return m_buffer;
    }

    void WriteArgs::SetBuffer(int fd)
    {
        cout << "msg: " << m_buffer << " here: " << __FILE__ << ". Line:" << __LINE__  << endl; //for tests
        //read(fd, m_buffer, m_length);
    }
}