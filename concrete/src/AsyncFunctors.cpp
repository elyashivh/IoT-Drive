#include <cstring>
#include <iostream>

#include "AsyncFunctors.hpp"
#include "Handleton.hpp"
#include "ResponseManager.hpp"

namespace ilrd_166_7
{
    AsyncFunctorRead::AsyncFunctorRead(const UID &uid, const char *handle)
        : m_uid(uid), m_counter(0)
    {
        memcpy(m_nbd_uid, handle, NBD_SIZE);
    }

    bool AsyncFunctorRead::operator()()
    {
        auto response_manager = Handleton<ResponseManager>::GetInstance();

        ++m_counter;

        if (!response_manager->IsUIDExists(m_uid))
        {
            return false;
        }
        else if (m_counter > FAULT_TOLERANCE)
        {
            std::cerr << "AsyncFunctorRead: FAILURE" << std::endl;
            Handleton<NBDProxy>::GetInstance()->StatusResponse(m_nbd_uid, 1);

            return false;
        }

        return true;
    }

    AsyncFunctorWrite::AsyncFunctorWrite(
        const UID &main_uid,
        const UID &backup_uid,
        const char *handle) : m_uid_1(main_uid), m_uid_2(backup_uid), m_counter(0)
    {
        memcpy(m_nbd_uid, handle, NBD_SIZE);
    }

    bool AsyncFunctorWrite::operator()()
    {
        auto response_manager = Handleton<ResponseManager>::GetInstance();

        ++m_counter;

        if (!response_manager->IsUIDExists(m_uid_1) && !response_manager->IsUIDExists(m_uid_2))
        {
            Handleton<NBDProxy>::GetInstance()->StatusResponse(m_nbd_uid, 0);

            return false;
        }
        else if (m_counter > FAULT_TOLERANCE)
        {
            std::cerr << "AsyncFunctorWrite: FAILURE" << std::endl;
            Handleton<NBDProxy>::GetInstance()->StatusResponse(m_nbd_uid, 1);

            return false;
        }

        return true;
    }
}
