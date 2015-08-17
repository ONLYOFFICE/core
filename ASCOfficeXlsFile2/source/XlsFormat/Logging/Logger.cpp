
#include "Logger.h"

#include <iostream>
#include <sstream>
#include <time.h>

Logger::Logger(const std::string& log_file)
: m_log(log_file.c_str())
{
}


Logger::~Logger()
{
}

void Logger::writeLine(const std::string& type, const std::string& str)
{
    static wchar_t time_stamp[16] ={};
#if defined(_WIN32) || defined(_WIN64)
    time_t now;
	tm local;
	time(&now);
	localtime_s(&local, &now);

	swprintf_s(time_stamp, 15, L"%02d:%02d:%02d ", local.tm_hour, local.tm_min, local.tm_sec);
#else
	m_log		<< time_stamp << std::wstring(type.begin(),type.end()) << ": " << std::wstring(str.begin(),str.end())  << std::endl;
	std::cout	<< time_stamp << type << ": " << str << std::endl;
#endif
}

void Logger::writeLine(const std::wstring& type, const std::wstring& str)
{
    static wchar_t time_stamp[16]={};
#if defined(_WIN32) || defined(_WIN64)
    time_t now;
	tm local;
	time(&now);
	localtime_s(&local, &now);

	swprintf_s(time_stamp, 15, L"%02d:%02d:%02d ", local.tm_hour, local.tm_min, local.tm_sec);
#else

	m_log		<< time_stamp << type << L": " << str << std::endl;
	std::wcout	<< time_stamp << type << L": " << str << std::endl;
#endif
}
Logger& Logger::getLogger()
{
	static Logger logger("AVSOfficeXlsFile.log");
	return logger;
}

