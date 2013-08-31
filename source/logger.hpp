///
///	Author:			Christian Magnerfelt
/// 
///	Description:	This is a singleton logger class. Logging can be done by using the 
///					logger macros LOGGER_INFO, LOGGER_ERROR and LOGGER_DEBUG.
///					It is thread safe to use this class as long as getInstance is called on the main thread before any 
///					logging occurs. The first call to getInstance will construct the logger however in order 
///					initialize the logger with logfile the init function must be called before any logging.
///
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdarg>
#include <cstdio>
#include <mutex>

namespace Logger
{
	class LoggerManager
	{
		public:
			~LoggerManager();
			// Retrieves the singleton instance of LoggerManager
			static LoggerManager & getInstance();

			/// Logs a format with a va list.
			/// USE MACRO FUNCTIONS INSTEAD OF THIS FUNCTION DIRECTLY
			void log(const char * format, ...);

			/// Opens a log file for logging. Returns false on fail. True if already initialized.
			bool init(const char * logFile);

			/// Closes a logfile for logging
			bool finalize();
		private:
			LoggerManager() : m_outFile(nullptr), m_mutex(), m_initialized(false){}
			 
			FILE * m_outFile;
			std::mutex m_mutex;
			bool m_initialized;

			// Do not allow copying
			LoggerManager(const LoggerManager &);
			LoggerManager & operator=(const LoggerManager &);
	};
}

/// Logs a single line
#define LOGGER_INFO(format, ...) do { \
Logger::LoggerManager::getInstance().log(format,__VA_ARGS__); \
}while(0) 


#ifdef _DEBUG
	/// Logs a single line including function name, file name and line number
	#define LOGGER_DEBUG(format, ...) do { \
	Logger::LoggerManager::getInstance().log(format,__VA_ARGS__); \
	Logger::LoggerManager::getInstance().log("function: %s\n%s\nline: %d",__FUNCTION__, __FILE__, __LINE__); \
	}while(0)
#else
	/// LOGGER_DEBUG outputs nothing in release mode
	#define LOGGER_DEBUG(format,...) do { \
	(void)sizeof(format); \
	}while(0)
#endif
/// Logs a single line including function name, file name and line number
#define LOGGER_ERROR(format, ...) do { \
Logger::LoggerManager::getInstance().log(format,__VA_ARGS__); \
Logger::LoggerManager::getInstance().log("function: %s\n%s\nline: %d",__FUNCTION__, __FILE__, __LINE__); \
}while(0)

#endif