
#pragma once

#include <mutex>
#include <atomic>
#include <cstdlib>

#ifdef I_AM_THE_REAL_HANDLETON_IMPLEMENTOR

namespace ilrd_166_7
{

    /**
     * @class Handleton
     * @brief Thread-safe Singleton template with lazy initialization.
     *
     * @tparam T The type for which a singleton instance is needed.
     *
     * This class ensures that only one instance of type T is created and shared
     * across the program. It uses double-checked locking and memory fences to ensure
     * thread safety. The singleton is created on first access and automatically deleted
     * at program exit.
     */
    template <typename T>
    class Handleton
    {
    public:
        /**
         * @brief Returns a pointer to the singleton instance.
         *
         * Creates the instance on first call, and always returns the same pointer.
         * Thread-safe with double-checked locking.
         *
         * @return T* Pointer to the singleton instance.
         */
        static T *GetInstance();

        Handleton() = delete;
        Handleton(const Handleton &other) = delete;
        Handleton &operator=(const Handleton &other) = delete;
        Handleton(Handleton &&) = delete;
        ~Handleton() = delete;

    private:
        static std::atomic<T *> m_instance;
        static std::mutex m_mutex;
        static void Deleter();

    }; // Handelton

    /**************** CLASS IMPLEMENTATION ***********/

    template <typename T>
    std::atomic<T *> Handleton<T>::m_instance;

    template <typename T>
    std::mutex Handleton<T>::m_mutex;

    template <typename T>
    void Handleton<T>::Deleter()
    {
        delete m_instance.load();
        m_instance.store(reinterpret_cast<T *>(0XCAFEBABE));
    }

    template <typename T>
    T *Handleton<T>::GetInstance()
    {
        T *temp = m_instance.load(std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_acquire);

        if (!temp)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            temp = m_instance.load(std::memory_order_relaxed);
            if (!temp)
            {
                temp = new T();
                atexit(Handleton<T>::Deleter);
                std::atomic_thread_fence(std::memory_order_release);
                m_instance.store(temp, std::memory_order_relaxed);
            }
        }
        return temp;
    }
} // namespace ilrd_166_7

/**
 * @def IMPLEMENT_HANDLETON(T)
 * @brief Explicitly instantiates the Handleton template for type T.
 *
 * This macro should be used in a `.cpp` file to ensure that the static members
 * are properly compiled and linked.
 */
#define IMPLEMENT_HANDLETON(T) template class Handleton<T>;

#else

namespace ilrd_166_7
{
    template <typename T>
    class Handleton
    {
    public:
        static T *GetInstance();
        Handleton() = delete;
        Handleton(const Handleton &other) = delete;
        Handleton &operator=(const Handleton &other) = delete;
        Handleton(Handleton &&) = delete;
        ~Handleton() = delete;

    private:
        static std::atomic<T *> m_instance;
        static std::mutex m_mutex;

    }; // Handelton
} // ilrd_166_7

#endif // I_AM_THE_REAL_HANDLETON_IMPLEMENTOR
