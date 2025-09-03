#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <string>

namespace ilrd_166_7
{
    class FileProxy
    {
    public:
        explicit FileProxy(const std::string &disk_name = "disk.txt");
        ~FileProxy();

        bool Read(u_int32_t offset, u_int32_t size, char *out_buffer);
        bool Write(u_int32_t offset, u_int32_t size, const char *in_buffer);

    private:
        int m_fd;
    }; // FileProxy
} // ilrd_166_7
