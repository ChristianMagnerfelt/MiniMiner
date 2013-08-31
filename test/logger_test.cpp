///
///	Author: Christian Magnerfelt
///
///	Description: Unit tests for Logger class
/// 
#include <logger.hpp>

#include <cassert>
#include <iostream>

bool testInfoLogging()
{
	std::cout << __FUNCTION__ << std::endl;
	Logger::LoggerManager::GetInstance().init("test.txt");

	// These lines should only print timestamp + string
	LOGGER_INFO("a simple string");
	LOGGER_INFO("%s string with string", "nice");
	LOGGER_INFO("%d%d%d%d diffrent ways to test", 1, 0, 0, 0);
	Logger::LoggerManager::GetInstance().finalize();
}
bool testDebugLogging()
{
	std::cout << __FUNCTION__ << std::endl;
	Logger::LoggerManager::GetInstance().init("test.txt");

	// These lines should print nothing in release mode
	LOGGER_DEBUG("a simple string");
	LOGGER_DEBUG("%s string with string", "nice");
	LOGGER_DEBUG("%d%d%d%d diffrent ways to test", 1, 0, 0, 0);
	Logger::LoggerManager::GetInstance().finalize();
}
bool testErrorLogging()
{
	std::cout << __FUNCTION__ << std::endl;
	Logger::LoggerManager::GetInstance().init("test.txt");
	
	// These lines should allways print timestamp + string + line + file
	LOGGER_ERROR("a simple string");
	LOGGER_ERROR("%s string with string", "nice");
	LOGGER_ERROR("%d%d%d%d diffrent ways to test", 1, 0, 0, 0);
	Logger::LoggerManager::GetInstance().finalize();
}
void main()
{
	assert(testInfoLogging());
	assert(testDebugLogging());
	assert(testErrorLogging());
}