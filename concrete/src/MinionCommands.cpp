#include "MinionCommands.hpp"
#include "MinionReadArgs.hpp"
#include "MinionWriteArgs.hpp"
#include "ResponseMessage.hpp"
#include "Handleton.hpp"
#include "MasterProxy.hpp"

namespace ilrd_166_7
{
    asyncInjectionFunctor FileReadCommand::Run(std::shared_ptr<IKeyArgs> args)
    {
        MinionReadArgs *read_args = static_cast<MinionReadArgs *>(args.get());

        auto master_proxy = Handleton<MasterProxy>::GetInstance();

        char *buffer = new char[read_args->GetSize()];
        auto status = master_proxy->GetFile().Read(
            read_args->GetOffset(),
            read_args->GetSize(),
            buffer);

        master_proxy->SendResponse(
            std::make_shared<ResponseReadMessage>(
                read_args->GetUid(),
                status,
                read_args->GetSize(),
                buffer));

        return nullptr;
    }

    std::shared_ptr<ICommand> FileReadCommand::CreateFileReadCommand()
    {
        return std::make_shared<FileReadCommand>();
    }

    asyncInjectionFunctor FileWriteCommand::Run(std::shared_ptr<IKeyArgs> args)
    {
        MinionWriteArgs *write_args = static_cast<MinionWriteArgs *>(args.get());

        auto master_proxy = Handleton<MasterProxy>::GetInstance();

        auto status = master_proxy->GetFile().Write(
            write_args->GetOffset(),
            write_args->GetSize(),
            write_args->GetBuffer());

        master_proxy->SendResponse(
            std::make_shared<ResponseWriteMessage>(write_args->GetUid(), status));

        return nullptr;
    }

    std::shared_ptr<ICommand> FileWriteCommand::CreateFileWriteCommand()
    {
        return std::make_shared<FileWriteCommand>();
    }
}
