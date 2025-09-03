#define I_AM_THE_REAL_HANDLETON_IMPLEMENTOR

#include "Handleton.hpp" 
#include "Factory.hpp"   
#include "Scheduler.hpp" 
#include "Framework.hpp" 
#include "RAIDManager.hpp" 
#include "ResponseManager.hpp"
#include "Message.hpp"
#include "ResponseMessage.hpp"
#include "NBDProxy.hpp"
#include "MasterProxy.hpp"

namespace ilrd_166_7
{
    using MyFactory = Factory<int, int, int>;
    IMPLEMENT_HANDLETON(MyFactory);

    using MyScheduler = Scheduler;
    IMPLEMENT_HANDLETON(MyScheduler);

    using MyFactory1 = Factory<ICommand, int>;
    IMPLEMENT_HANDLETON(MyFactory1);

    using MyLogger = Logger;
    IMPLEMENT_HANDLETON(MyLogger);

    IMPLEMENT_HANDLETON(RAIDManager);

    using MyMessage1 = Factory<AbstractMessage, uint32_t>;
    IMPLEMENT_HANDLETON(MyMessage1);

    using MyMessage2 = Factory<AbstractMessage, int>;
    IMPLEMENT_HANDLETON(MyMessage2);

    using MyResponseManagerAndInt = Factory<ResponseManager, int>;
    IMPLEMENT_HANDLETON(MyResponseManagerAndInt);

    using pair1 = Factory<AbstractResponseMessage, AbstractResponseMessage::MSG_TYPE>;
    IMPLEMENT_HANDLETON(pair1);

    using pair2 = Factory<AbstractMessage, AbstractMessage::MSG_TYPE>;
    IMPLEMENT_HANDLETON(pair2);

    IMPLEMENT_HANDLETON(NBDProxy);
    IMPLEMENT_HANDLETON(ResponseManager);
    IMPLEMENT_HANDLETON(MasterProxy);

}
