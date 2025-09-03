#pragma once

#include "UID.hpp"

namespace ilrd_166_7
{
    struct AsyncFunctorRead
    {
    public:
        static const int FAULT_TOLERANCE = 30;
        const static unsigned int NBD_SIZE = 8;

        AsyncFunctorRead(const UID &uid, const char *handle);

        bool operator()();

    private:
        UID m_uid;
        int m_counter;
        char m_nbd_uid[NBD_SIZE];
    }; // AsyncFunctorRead

    struct AsyncFunctorWrite
    {
    public:
        static const int FAULT_TOLERANCE = 30;
        const static unsigned int NBD_SIZE = 8;

        AsyncFunctorWrite(const UID &main_uid, const UID &backup_uid, const char *handle);

        bool operator()();

    private:
        UID m_uid_1;
        UID m_uid_2;
        int m_counter;
        char m_nbd_uid[NBD_SIZE];
    }; // AsyncFunctorWrite
} // ilrd_166_7
