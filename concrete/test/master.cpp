#include <iostream>

#include "Framework.hpp"
#include "NBDProxy.hpp"
#include "RAIDManager.hpp"
#include "ReadWriteCommand.hpp"
#include "Handleton.hpp"
#include "Factory.hpp"
#include "ResponseMessage.hpp"

using namespace std;
using namespace ilrd_166_7;

shared_ptr<AbstractResponseMessage> CreateResponseReadMessage()
{
    return make_shared<ResponseReadMessage>();
}

shared_ptr<AbstractResponseMessage> CreateResponseWriteMessage()
{
    return make_shared<ResponseWriteMessage>();
}

int main()
{
    cout << "Master started...\n";

    auto nbd_proxy = Handleton<NBDProxy>::GetInstance();

    list<pair<fdAndMode, IInputProxy*>> plist;
    list<pair<int, function<shared_ptr<ICommand>()>>> clist;
    LinuxListener listener;

    int nbd_fd = nbd_proxy->GetFD();

    plist.push_back({{nbd_fd, Mode::READ}, nbd_proxy});
    plist.push_back({{nbd_fd, Mode::WRITE}, nbd_proxy});

    clist.push_back({0, &ReadCommand::CreateReadCommand});
    clist.push_back({1, &WriteCommand::CreateWriteCommand});

    auto factory = Handleton<Factory<AbstractResponseMessage, AbstractResponseMessage::MSG_TYPE>>::GetInstance();

    factory->Add(AbstractResponseMessage::READ, &CreateResponseReadMessage);
    factory->Add(AbstractResponseMessage::WRITE, &CreateResponseWriteMessage);

    Framework framework(&listener, plist, clist, "/home/user/git/projects/final_project/framework/plugins");
    framework.Run();

    return EXIT_SUCCESS;
}
