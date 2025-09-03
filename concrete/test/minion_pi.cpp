#include <iostream>

#include "framework.hpp"
#include "master_proxy.hpp"
#include "handleton.hpp"
#include "factory.hpp"
#include "minion_commands.hpp"
#include "message.hpp"

using namespace ilrd_166_7;

std::shared_ptr<AbstractMessage> CreateReadMessage()
{
    return std::make_shared<ReadMessage>();
}

std::shared_ptr<AbstractMessage> CreateWriteMessage()
{
    return std::make_shared<WriteMessage>();
}

int main()
{
    std::cout << "Pi started...\n";

    auto master_proxy = Handleton<MasterProxy>::GetInstance();
    Framework::proxy_list_t plist;
    Framework::create_func_list_t clist;
    
    auto factory = Handleton<Factory<AbstractMessage, AbstractMessage::MSG_TYPE>>::GetInstance();
    
    factory->Add(AbstractMessage::READ, &CreateReadMessage);
    factory->Add(AbstractMessage::WRITE, &CreateWriteMessage);
    
    u_int16_t udp = master_proxy->GetUDP();
    
    Reactor::fd_mode udp_pair = {udp, Reactor::MODE::READ};
    plist.push_back({udp_pair, master_proxy});
    clist.push_back({0, &FileReadCommand::CreateFileReadCommand});
    clist.push_back({1, &FileWriteCommand::CreateFileWriteCommand});

    Framework framework("./plugins", plist, clist);
    framework.Run();

    return EXIT_SUCCESS;
}