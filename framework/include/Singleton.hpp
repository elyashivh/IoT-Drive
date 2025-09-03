#pragma once

#include <mutex>
#include <atomic>
#include <cstdlib>

namespace ilrd_166_7
{

    /**
     * @class Singleton
     * @brief Template class for creating thread-safe Singleton instances.
     *
     * The Singleton class ensures that only one instance of a given type `T`
     * is created during the lifetime of the program. This implementation is
     * thread-safe using double-checked locking with memory fences and `std::mutex`.
     *
     * The created instance is automatically destroyed at program termination
     * using `std::atexit()`.
     *
     * @tparam T The class type to be managed as a Singleton.
     */
    template <typename T>
    class Singleton
    {
    public:
        static T *GetInstance();
        Singleton() = delete;
        Singleton(const Singleton &other) = delete;
        Singleton &operator=(const Singleton &other) = delete;
        ~Singleton() = delete;

    private:
        static T *m_instance;
        static std::mutex m_mutex;

        static void Deleter();

    }; // singleton

    /************* CLASS IMPLEMENTATION *********** */
    template <typename T>
    T *Singleton<T>::m_instance = nullptr;

    template <typename T>
    std::mutex Singleton<T>::m_mutex;

    template <typename T>
    void Singleton<T>::Deleter()
    {
        delete m_instance;
    }

    template <typename T>
    T *Singleton<T>::GetInstance()
    {
        std::atomic_thread_fence(std::memory_order_acquire);
        if (m_instance == nullptr)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_instance == nullptr)
            {
                m_instance = new T;
                atexit(Deleter);
                std::atomic_thread_fence(std::memory_order_release);
            }
        }

        return m_instance;
    }

} // ilrd namespace