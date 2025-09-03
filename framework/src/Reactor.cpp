#include <sys/select.h>     //select & FD_ZERO & FD_SET
#include <iostream>

#include "Reactor.hpp"
using namespace std;

namespace std
{
    template <>
    struct hash<ilrd_166_7::fdAndMode> 
    {
        std::size_t operator()(const ilrd_166_7::fdAndMode &pair) const
        {
            return std::hash<int>()(pair.first) ^
                 (std::hash<int>()(static_cast<int>(pair.second)) << 1);
        }
    };
}


namespace ilrd_166_7
{
/**************FORWARD DECLARATON**************** */
static int SetFdsSet(fd_set& readFds, fd_set& writeFds, std::map<fdAndMode,
                                     std::function<void()>> &fdModeToCB);
template<typename T>
static T Max(T a, T b);
static funcVector SetFuncVec(fd_set& readFds, fd_set& writeFds, std::map<fdAndMode,
                                             std::function<void()>> &fdModeToCB);




/*******CLASS METHODS IMP********/
Reactor::Reactor(IListener* pListener): m_pListener(pListener)
{
    //empty
}

void Reactor::Register(int fd,Mode mode, std::function<void(int,Mode)> callback)
{
    m_fdModeToCB[fdAndMode(fd, mode)] = bind(callback, fd, mode);
}

void Reactor::Unregister(int fd,Mode mode)
{
    m_fdModeToCB.erase(fdAndMode(fd, mode));
}

void Reactor::Run()
{
    cout<< "Reactor: " << __FILE__ << ". Line:" << __LINE__  << endl;
    m_running = true;

    while (m_running)
    {
        funcVector vec = m_pListener->Listen(m_fdModeToCB);
        for (auto functor : vec)
        {
            functor();
        }
    }
}

void Reactor::Stop()
{
    m_running = false;
}

funcVector LinuxListener::Listen(std::map<fdAndMode, std::function<void()>> &fdModeToCB)
{
    fd_set readFds, writeFds;
    int maxFd = SetFdsSet(readFds, writeFds, fdModeToCB);
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    int ready = select(maxFd+1, &readFds, &writeFds, nullptr, &timeout);
    if (ready == -1)
    {
        throw runtime_error("Select failed");
    }
    if (ready == 0)
    {
        funcVector retVal;
        return retVal;
    }

    return SetFuncVec(readFds,writeFds, fdModeToCB);
}

static funcVector SetFuncVec(fd_set& readFds, fd_set& writeFds, std::map<fdAndMode,
                                             std::function<void()>> &fdModeToCB)
{
    funcVector retVal;
    
    for (auto it : fdModeToCB)
    {
        int currFd = it.first.first;
        Mode currMode = it.first.second;

        if (FD_ISSET(currFd, &readFds) || FD_ISSET(currFd, &writeFds))
        {
            retVal.push_back(fdModeToCB[fdAndMode(currFd, currMode)]);
        }
    }

    return retVal;
}

static int SetFdsSet(fd_set& readFds, fd_set& writeFds, std::map<fdAndMode,
                                                 std::function<void()>> &fdModeToCB)
{
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);
    int maxFd = 0;

    for (auto it : fdModeToCB)
    {
        int currFd = it.first.first;
        maxFd = Max(maxFd, currFd);
        Mode currMode = it.first.second;
        if (currMode == Mode::READ)
        {
            FD_SET(currFd, &readFds);
        }
        else if (currMode == Mode::WRITE)
        {
            FD_SET(currFd, &writeFds);
        }
        else
        {
            FD_SET(currFd, &readFds);
            FD_SET(currFd, &writeFds);
        }
    }
    return maxFd;
}
 
template<typename T>
static T Max(T a, T b)
{
    return (a > b ? a : b);
} 

}   //end namespace
