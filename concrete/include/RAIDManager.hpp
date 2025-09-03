#pragma once

#include <vector>
#include <memory>

#include "IMinionProxy.hpp"

namespace ilrd_166_7
{
    class RAIDManager
    {
    public:
        explicit RAIDManager();

        struct Minions
        {
            std::shared_ptr<IMinionProxy> m_primaryMinion;
            std::shared_ptr<IMinionProxy> m_backupMinion;
            size_t m_primaryOffset;
            size_t m_backupOffset;
        };

        Minions GetMinionProxy(size_t offset);

    private:
        std::vector<std::shared_ptr<IMinionProxy>> m_minions;
    }; // RaidManager
} // ilrd_166_7
