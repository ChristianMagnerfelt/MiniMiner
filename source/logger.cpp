///
///	Author:			Christian Magnerfelt
///
#include "logger.hpp"

#include <ctime>
#include <memory>

Logger::LoggerManager::~LoggerManager()
{
	// Close the log file
	finalize();
}
// Prints a string format with variable number of parameters. Every string is prepended with current date and time.
void Logger::LoggerManager::log(const char * format, ...)
{
	va_list ap;
	va_start(ap, format);

	// Get the current timestamp
	time_t tp = std::time(nullptr);
	tm * dateTime = localtime(&tp);

	// Delay locking as long as possible
	std::lock_guard<std::mutex> lock(m_mutex);

	// Print time stamp
	fprintf(m_outFile, "%d:%d:%d %d:%d:%d : ", dateTime->tm_year + 1900, dateTime->tm_mon + 1, dateTime->tm_mday, dateTime->tm_hour, dateTime->tm_min, dateTime->tm_sec);

	// Print log
	vfprintf(m_outFile, format, ap);
	fprintf(m_outFile, "\n");

	fflush(m_outFile);
	va_end(ap);
}
/// Returns the singleton instance of LoggerManager
Logger::LoggerManager & Logger::LoggerManager::getInstance()
{
	static LoggerManager loggerManager;
	return loggerManager;
}
/// Opens a logFile for logging
bool Logger::LoggerManager::init(const char * logFile)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if(!m_initialized)
	{
		m_outFile = fopen(logFile, "w");
		if(m_outFile == nullptr)
		{
			// Could not open log file
			return false;
		}
		m_initialized = true;
	}

	return m_initialized;
}
/// Closes the logfile for logging
bool Logger::LoggerManager::finalize()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if(m_outFile != nullptr)
	{
		fclose(m_outFile);
	}
	m_initialized = false;
	return true;
}