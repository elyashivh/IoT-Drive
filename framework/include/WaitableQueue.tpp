#ifndef __WAITABLE_QUEUE_TPP__
#define __WAITABLE_QUEUE_TPP__

namespace ilrd_166_7
{

template <typename T, typename CONTAINER>
void WaitableQueue<T, CONTAINER>::Pop(T& out_param)
{
    std::unique_lock<std::mutex> lock (m_mutatorLock);
    m_cv.wait(lock,
        [this]
        {
            return !(m_container.empty());
        });
    out_param = m_container.front();
    m_container.pop();
}

template <typename T, typename CONTAINER>
bool WaitableQueue<T, CONTAINER>::Pop(T& out_param,
                                      std::chrono::milliseconds timeout)
{
    std::unique_lock<std::mutex> lock (m_mutatorLock);
    if (!m_cv.wait_for(lock, timeout,
         [this]
         { 
            return !m_container.empty(); 
        })) 
    {
        return false;
    }
    out_param = m_container.front();
    m_container.pop();
    return true;
}

template <typename T, typename CONTAINER>
void WaitableQueue<T, CONTAINER>::Push(const T& data)
{
    std::unique_lock<std::mutex> lock (m_mutatorLock);
    m_container.push(data);
    m_cv.notify_one();
}

template <typename T, typename CONTAINER>
bool WaitableQueue<T, CONTAINER>::IsEmpty() const noexcept
{
    std::lock_guard<std::mutex> lock (m_mutatorLock);

    return m_container.empty();
}

}   // namespace ilrd_166_7

#endif  // __WAITABLE_QUEUE_TPP__
