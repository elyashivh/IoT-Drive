#pragma once

#include <memory> //shared_ptr

#include "Framework.hpp"

namespace ilrd_166_7
{
    class ReadCommand : public ICommand
    {
    public:
        asyncInjectionFunctor Run(std::shared_ptr<IKeyArgs> args) override;
        static std::shared_ptr<ICommand> CreateReadCommand();
    }; // ReadCommand

    class WriteCommand : public ICommand
    {
    public:
        asyncInjectionFunctor Run(std::shared_ptr<IKeyArgs> args) override;
        static std::shared_ptr<ICommand> CreateWriteCommand();
    }; // WriteCommand

} // ilrd_166_7
