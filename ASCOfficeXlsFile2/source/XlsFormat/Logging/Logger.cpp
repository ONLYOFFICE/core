
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
	std::cout	<< type << ": " << str << std::endl;
}

void Logger::writeLine(const std::wstring& type, const std::wstring& str)
{
	std::wcout	<< type << L": " << str << std::endl;

}
Logger& Logger::getLogger()
{
	static Logger logger("OfficeXlsFile.log");
	return logger;
}

