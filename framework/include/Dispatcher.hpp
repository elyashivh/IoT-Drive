#pragma once

#include <functional>
#include <unordered_set>

namespace ilrd_166_7
{
    /*******FORWARD DECLARATION*******/
    template <typename EVENT>
    class Dispatcher;

    /**
     * @brief Abstract base class for callback handlers used in the Dispatcher pattern.
     *
     * @tparam EVENT The type of the event to be dispatched.
     *
     * This class provides a base interface for objects that wish to receive events
     * from a Dispatcher. It supports both event notification and optional death notification.
     */
    template <typename EVENT>
    class ACallback
    {
    public:
        ACallback() : m_dispathcer(nullptr) { /*empty*/ }
        virtual ~ACallback()
        {
            if (m_dispathcer)
            {
                m_dispathcer->Unregister(this);
            }
        }

        /**
         * @brief Pure virtual method to handle an incoming event.
         *
         * @param event The event to handle.
         */
        virtual void Notify(const EVENT&) = 0;

        /**
         * @brief Optional method called when the dispatcher is destroyed.
         *
         * Can be overridden by derived classes to perform cleanup logic.
         */
        virtual void NotifyDeath() {/*empty*/}

        /**
         * @brief Sets the dispatcher reference for automatic unregistration.
         *
         * @param disp A pointer to the Dispatcher instance.
         */
        void SetDispatcher(Dispatcher<EVENT> *disp)
        {
            m_dispathcer = disp;
        }

    protected:
        Dispatcher<EVENT> *m_dispathcer;
    }; // end ACallBack

    /**
     * @brief Concrete callback class that binds a member function to an event.
     *
     * @tparam EVENT The event type.
     * @tparam OBSERVER The observer (listener) class type.
     */
    template <typename EVENT, typename OBSERVER>
    class Callback : public ACallback<EVENT>
    {
    public:
        /**
         * @brief Constructs a callback with given observer and notification functions.
         *
         * @param obj Pointer to the observer instance.
         * @param notify Function to call upon event notification.
         * @param death Optional function to call upon dispatcher destruction.
         */
        Callback(OBSERVER *obj,
                 std::function<void(OBSERVER *, EVENT)> notify,
                 std::function<void(OBSERVER *)> death = nullptr)
            : m_observer(obj), m_notifyFunc(notify), m_deathFunc(death) {/*empty*/}

        /**
         *
         * @brief Executes the notification function with the given event.
         *
         * @param event The dispatched event.
         */
        virtual void Notify(const EVENT& event)
        {
            m_notifyFunc(m_observer, event);
        }

        /**
         * @brief Executes the death function if it exists.
         */
        virtual void NotifyDeath()
        {
            if (m_deathFunc)
            {
                m_deathFunc(m_observer);
            }
        }

    private:
        OBSERVER *m_observer;
        std::function<void(OBSERVER *, EVENT)> m_notifyFunc;
        std::function<void(OBSERVER *)> m_deathFunc;

    }; // end CallBack


    /**
     * @brief Dispatcher class for managing and notifying callbacks.
     *
     * @tparam EVENT The event type to be dispatched.
     *
     * Dispatcher holds a set of callback listeners and allows broadcasting events
     * to all registered listeners.
     */
    template <typename EVENT>
    class Dispatcher
    {
    public:
        /* @brief Registers/Unregisters a callback to receive events.
         *
         * @param callback Pointer to the callback to register.
         */
        void Register(ACallback<EVENT> *callback);
        void Unregister(ACallback<EVENT> *ACallback);

        /**
         * @brief Notifies all registered callbacks of an event.
         *
         * @param event The event to dispatch.
         */
        void Notify(const EVENT& event);
        ~Dispatcher();

    private:
        std::unordered_set<ACallback<EVENT> *> m_set;
    }; // end Dispatcher


    /**********CLASS IMPLEMENTATION************/
    template <typename EVENT>
    void Dispatcher<EVENT>::Register(ACallback<EVENT> *callback)
    {
        m_set.insert(callback);
    }

    template <typename EVENT>
    void Dispatcher<EVENT>::Unregister(ACallback<EVENT> *ACallback)
    {
        m_set.erase(ACallback);
    }

    template <typename EVENT>
    void Dispatcher<EVENT>::Notify(const EVENT& event)
    {
        std::unordered_set<ACallback<EVENT> *> temp = m_set;
        for (auto it : temp)
        {
            it->Notify(event);
        }
    }

    template <typename EVENT>
    Dispatcher<EVENT>::~Dispatcher()
    {
        std::unordered_set<ACallback<EVENT> *> temp = m_set;
        for (auto it : temp)
        {
            it->NotifyDeath();
        }
    }
} // end namespace ilrd_166_7
