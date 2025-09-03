#pragma once

#include <string>
#include <mutex>
#include <thread>

#include "Dispatcher.hpp"
#include "Logger.hpp"

namespace ilrd_166_7
{

	/****** FORWARD DECLARATION ********/
	class DllLoader;

	
	/**
	 * @class DirMonitor
	 * @brief Monitors a directory for new shared library files and dispatches events.
	 *
	 * Uses the inotify Linux subsystem to watch a directory for changes (e.g., new .so files).
	 * When a relevant file is detected, it notifies all registered observers (via Dispatcher).
	 */
	class DirMonitor
	{
	public:
		/**
		 * @brief Constructs a DirMonitor for the specified directory.
		 *
		 * @param pathName Absolute path of the directory to monitor.
		 */
		DirMonitor(const std::string& pathName);
		~DirMonitor();

		/**
		 * @brief Registers/UnRegister a callback to be notified of new file events.
		 *
		 * @param callBack Pointer to a callback object.
		 */
		void Register(ACallback<std::string> *callBack);
		void Unregister(ACallback<std::string> *callBack);

		/**
		 * @brief Provides access to the internal dispatcher.
		 *
		 * @return Pointer to the dispatcher used for notifying callbacks.
		 */
		Dispatcher<std::string> *GetDispatcher();

		/**
		 * @brief Starts the monitoring thread.
		 *
		 * This function begins the background loop which waits for new files
		 * and dispatches events accordingly.
		 */
		void Run();

	private:
		Dispatcher<std::string> m_dispatcher;
		std::string m_path;
		std::mutex m_mutex;
		std::thread m_thread;
		std::atomic_bool m_running;
		int m_inotifyFd;
		int m_wFd;
		void RunMonitorThread();
		Dispatcher<std::string> &GetDisatcher();

	}; // end DirMonitor


	/**
	 * @class DllLoader
	 * @brief Responsible for dynamically loading shared libraries (.so files).
	 *
	 * This class provides functionality for loading plugins or modules
	 * during runtime using the file path provided.
	 */
	class DllLoader
	{
	public:
		/**
		 * @brief Loads a shared object file into the program.
		 *
		 * @param fileName Absolute path to the .so file.
		 */
		void Load(const std::string& fileName);
	}; // end DllLoader

} // end ilrd_166_7