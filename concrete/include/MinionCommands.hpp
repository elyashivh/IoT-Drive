#pragma once

#include "Framework.hpp"

namespace ilrd_166_7
{
    class FileReadCommand : public ICommand
    {
    public:
        asyncInjectionFunctor Run(std::shared_ptr<IKeyArgs> args) override;
        static std::shared_ptr<ICommand> CreateFileReadCommand();
    }; // FileReadCommand

    class FileWriteCommand : public ICommand
    {
    public:
        asyncInjectionFunctor Run(std::shared_ptr<IKeyArgs> args) override;
        static std::shared_ptr<ICommand> CreateFileWriteCommand();
    }; // FileWriteCommand
} // ilrd_166_7
