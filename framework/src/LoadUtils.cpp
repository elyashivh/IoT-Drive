#include <stdexcept>        //runtime_error
#include <vector>           // std::vector
#include <sys/inotify.h>    //inotify  
#include <poll.h>           //poll
#include <unistd.h>         //read
#include <dlfcn.h>          // dlopen

#include "LoadUtils.hpp"

using namespace std;

namespace ilrd_166_7
{
static const char Slash = '/';
static const size_t BUFFER_SIZE = 1024;
static const size_t TIMEOUT = 100;

DirMonitor::DirMonitor(const string& pathName): m_path(pathName)
{
    m_inotifyFd = inotify_init();
    if (m_inotifyFd == -1)
    {
        throw runtime_error("inotify init failed");
    }

    m_wFd = inotify_add_watch(m_inotifyFd, m_path.c_str(),
                                    IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_CLOSE_WRITE);
    if (m_wFd == -1)
    {
        close(m_inotifyFd);
        throw runtime_error("inotify add watch failed");
    }
}

DirMonitor::~DirMonitor()
{
    m_running.store(false);
    m_thread.join();
}

void DirMonitor::Register(ACallback<std::string>* callBack)
{
    lock_guard<mutex> lock(m_mutex);
    m_dispatcher.Register(callBack);
}

void DirMonitor::Unregister(ACallback<std::string>* callBack)
{
    lock_guard<mutex> lock(m_mutex);
    m_dispatcher.Unregister(callBack);
}

void DirMonitor::Run()
{
    m_running.store(true);
    m_thread = thread(&DirMonitor::RunMonitorThread, this);
}

Dispatcher<std::string>& DirMonitor::GetDisatcher()
{
    return m_dispatcher;
}
void DirMonitor::RunMonitorThread()
{
    pollfd fds;
    fds.fd = m_inotifyFd;
    fds.events = POLLIN;
    
    vector<char> buffer(BUFFER_SIZE);

    while (m_running.load())
    {
        int ready = poll(&fds, 1, TIMEOUT);

        if (ready > 0 && (fds.revents & POLLIN))
        {
            int len = read(m_inotifyFd, buffer.data(), buffer.size());

            if (len > 0)
            {
                char* data = buffer.data();
                while (data < buffer.data() + len)
                {
                    struct inotify_event* event = reinterpret_cast<struct inotify_event*>(data);
    
                    if (event->mask & (IN_CREATE | IN_MOVED_TO | IN_CLOSE_WRITE))
                    {
                        m_dispatcher.Notify(m_path + Slash + event->name);
                    }
    
                    if (event->mask & IN_DELETE)
                    {
                        // empty
                    }
    
                    data += sizeof(inotify_event) + event->len;

                }
            }
        }
    }
}



void DllLoader::Load(const string& fileName)
{
    void* file = dlopen(fileName.c_str(), RTLD_NOW);

    if (!file)
    {
        throw runtime_error("open file failed");
    }
}


}   //end namespace ilrd_166_7