#include <iostream>
#include <mutex>

#include "NBDProxy.hpp"
#include "NetworkUtils.h"
#include "colors.hpp"

using namespace std;
namespace ilrd_166_7
{

    shared_ptr<IKeyArgs> NBDProxy::GetKeyArgs(fdAndMode key)
    {
        (void)key;

        return (m_nbd.GetKeyArgs());
    }

    int NBDProxy::GetFD()
    {
        return (m_nbd.GetFD());
    }

    void NBDProxy::ReadResponse(char nbdUid[8], uint32_t size, const char *buffer)
    {
        m_nbd.Respond(nbdUid, size, buffer);
    }

    void NBDProxy::StatusResponse(char nbdUid[8], uint8_t status)
    {
        m_nbd.Respond(nbdUid, status);
    }
}
