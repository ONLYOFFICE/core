#pragma once
#ifndef UTILITY_LOG_INCLUDE_H_
#define UTILITY_LOG_INCLUDE_H_

#include <string>

class Log
{
public:
	static void event(const std::string& message);
	static void event(const std::wstring& message);
	static void message(const std::string& message);
	static void message(const std::wstring& message);
	static void warning(const std::string& message);
	static void warning(const std::wstring& message);
	static void error(const std::string& message);
	static void error(const std::wstring& message);
};

#endif // UTILITY_LOG_INCLUDE_H_