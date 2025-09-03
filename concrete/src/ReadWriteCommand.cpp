#include <iostream>
#include <cstring>

#include "MasterWriteArgs.hpp"
#include "MasterReadArgs.hpp"
#include "ReadWriteCommand.hpp"
#include "Handleton.hpp"
#include "ResponseManager.hpp"
#include "RAIDManager.hpp"
#include "UID.hpp"
#include "AsyncFunctors.hpp"

using namespace std;

namespace ilrd_166_7
{
    asyncInjectionFunctor ReadCommand::Run(std::shared_ptr<IKeyArgs> args)
    {
        auto read_args = std::dynamic_pointer_cast<MasterReadArgs>(args);

        auto data = Handleton<RAIDManager>::GetInstance()->GetMinionProxy(read_args->GetOffset());

        UID command_uid;

        Handleton<ResponseManager>::GetInstance()->RegisterUID(command_uid, read_args->GetUid());

        data.m_primaryMinion->Read(command_uid, data.m_primaryOffset, read_args->GetSize());

        return AsyncFunctorRead(command_uid, read_args->GetUid());
    }

    asyncInjectionFunctor WriteCommand::Run(std::shared_ptr<IKeyArgs> args)
    {
        auto write_args = std::dynamic_pointer_cast<MasterWriteArgs>(args);

        auto data = Handleton<RAIDManager>::GetInstance()->GetMinionProxy(write_args->GetOffset());

        UID command_uid_1;
        UID command_uid_2;

        auto response_manager = Handleton<ResponseManager>::GetInstance();
        response_manager->RegisterUID(command_uid_1, write_args->GetUid());
        response_manager->RegisterUID(command_uid_2, write_args->GetUid());

        data.m_primaryMinion->Write(
            command_uid_1,
            data.m_primaryOffset,
            write_args->GetSize(),
            write_args->GetBuffer());

        data.m_backupMinion->Write(
            command_uid_2,
            data.m_backupOffset,
            write_args->GetSize(),
            write_args->GetBuffer());

        return AsyncFunctorWrite(command_uid_1, command_uid_2, write_args->GetUid());
    }

    std::shared_ptr<ICommand> ReadCommand::CreateReadCommand()
    {
        return std::make_shared<ReadCommand>();
    }

    std::shared_ptr<ICommand> WriteCommand::CreateWriteCommand()
    {
        return std::make_shared<WriteCommand>();
    }

} // ilrd_166_7