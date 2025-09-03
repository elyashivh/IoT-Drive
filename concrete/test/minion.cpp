#include <iostream>

#include "Framework.hpp"
#include "MasterProxy.hpp"
#include "Handleton.hpp"
#include "Factory.hpp"
#include "MinionCommands.hpp"
#include "Message.hpp"

using namespace std;
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
    std::cout << "Slave started...\n";

    auto master_proxy = Handleton<MasterProxy>::GetInstance();

    list<pair<fdAndMode, IInputProxy*>> plist;
    list<pair<int, function<shared_ptr<ICommand>()>>> clist;
    LinuxListener listener;

    auto factory = Handleton<Factory<AbstractMessage, AbstractMessage::MSG_TYPE>>::GetInstance();

    factory->Add(AbstractMessage::READ, &CreateReadMessage);
    factory->Add(AbstractMessage::WRITE, &CreateWriteMessage);

    u_int16_t udp = master_proxy->GetUDP();

    fdAndMode udp_pair = {udp, Mode::READ};
    plist.push_back({udp_pair, master_proxy});
    clist.push_back({0, &FileReadCommand::CreateFileReadCommand});
    clist.push_back({1, &FileWriteCommand::CreateFileWriteCommand});

    Framework framework(&listener, plist, clist, "/home/user/git/projects/final_project/framework/plugins");
    framework.Run();

    return EXIT_SUCCESS;
}