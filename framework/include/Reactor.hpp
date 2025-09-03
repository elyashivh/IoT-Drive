#pragma once

#include <vector>
#include <functional>
#include <map>

namespace ilrd_166_7
{

    /**
     * @enum Mode
     * @brief Describes file descriptor operation modes.
     */
    enum class Mode
    {
        READ,
        WRITE,
        READ_AND_WRITE
    };

    using fdAndMode = std::pair<int, Mode>;
    using funcVector = std::vector<std::function<void()>>;

    /**
     * @class IListener
     * @brief Abstract interface for OS-specific listener implementations.
     *
     * The listener is responsible for monitoring registered file descriptors
     * and returning a list of callbacks that need to be executed.
     */
    class IListener
    {
    public:
        virtual funcVector Listen(std::map<fdAndMode, std::function<void()>> &fdModeToCB) = 0;
        virtual ~IListener() = default;
    }; // end IListener


    /**
     * @class LinuxListener
     * @brief Linux-specific implementation of IListener using system calls like select/poll.
     */
    class LinuxListener : public IListener
    {
    public:
        /**
         * @brief Monitors file descriptors using Linux-specific mechanisms (e.g., `select()` or `poll()`).
         * @param fdModeToCB Map of file descriptors and their corresponding callbacks.
         * @return Vector of callbacks ready to be executed.
         */
        funcVector Listen(std::map<fdAndMode, std::function<void()>> &fdModeToCB) override;
        virtual ~LinuxListener() = default;
    }; // end LinuxListener

    
    /**
     * @class Reactor
     * @brief Event-driven dispatcher for file descriptor-based events.
     *
     * The Reactor pattern handles multiple sources of events (like sockets or files)
     * by using an IListener to detect activity and invoking registered callbacks accordingly.
     */
    class Reactor
    {
    public:
        Reactor(IListener *pListener);

        void Register(int fd, Mode mode, std::function<void(int, Mode)> callback); // May be called from callbacks
        void Unregister(int fd, Mode mode);                                        // May be called from callbacks
        
        void Run();
        void Stop();

    private:
        IListener *m_pListener;
        std::map<fdAndMode, std::function<void()>> m_fdModeToCB;
        bool m_running;

    }; // end Reactor

} // end namespace ilrd_166_7
