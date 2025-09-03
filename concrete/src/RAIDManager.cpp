#include "MinionProxy.hpp"
#include "RAIDManager.hpp"

namespace ilrd_166_7
{
    static const size_t MB = 1 << 20;

    static const int MINION_NUM = 3;
    static const int MINION_SIZE = 8 * MB;
    static const int BLOCK_SIZE = 4 * MB;
    static const int PORT_0 = 8000;
    // static const char *PI_IP = "10.1.0.5";

    RAIDManager::RAIDManager()
    {
        int i = 0;

        for (; i < MINION_NUM; ++i)
        {
            m_minions.push_back(std::make_shared<MinionProxy>(PORT_0 + (i * 100)));
        }
    }

    RAIDManager::Minions RAIDManager::GetMinionProxy(size_t offset)
    {
        Minions result = {};

        result.m_primaryMinion = m_minions[(offset / BLOCK_SIZE)];
        result.m_primaryOffset = offset % BLOCK_SIZE;

        result.m_backupMinion = m_minions[((offset / BLOCK_SIZE) + 1) % MINION_NUM];
        result.m_backupOffset = (offset % BLOCK_SIZE) + BLOCK_SIZE;

        return result;
    }
}
