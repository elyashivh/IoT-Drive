#ifndef __ILRD_WAITABLE_QUEUE_HPP__
#define __ILRD_WAITABLE_QUEUE_HPP__

#include <queue>
#include <mutex>
#include <condition_variable>

namespace ilrd_166_7
{

    /**
     * @class WaitableQueue
     * @brief Thread-safe queue with blocking and timeout-aware pop operations.
     *
     * A generic wrapper around a container (default: std::queue) that allows
     * multiple threads to safely push and pop elements. Supports blocking
     * pops and timed pops.
     *
     * @tparam T Type of elements stored in the queue.
     * @tparam CONTAINER Underlying container type (defaults to std::queue<T>).
     *
     * @note Type T must be copy-constructible or move-constructible.
     */
    template <typename T, typename CONTAINER = std::queue<T>>
    class WaitableQueue
    {
    public:
        explicit WaitableQueue() = default;
        WaitableQueue(const WaitableQueue &other) = delete;
        WaitableQueue &operator=(const WaitableQueue &other) = delete;
        ~WaitableQueue() = default;

        void Pop(T &out_param);
        bool Pop(T &out_param, std::chrono::milliseconds timeout);
        void Push(const T &data);
        bool IsEmpty() const noexcept; // not MT-safe

        // The T type must implement move and copy ctor
    private:
        mutable std::mutex m_mutatorLock;
        std::condition_variable m_cv;
        CONTAINER m_container;
    };
} // namespace ilrd_166_7

#include "WaitableQueue.tpp"

#endif // __ILRD_WAITABLE_QUEUE_HPP__