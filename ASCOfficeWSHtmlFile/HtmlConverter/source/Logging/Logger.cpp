#include "stdafx.h"
#include "Logger.h"


Logger::Logger(const std::string& log_file)
: m_log(log_file.c_str())
{
}


Logger::~Logger()
{
}

void Logger::writeLine(const std::string& type, const std::string& str)
{
	time_t now;
	tm local;
	time(&now);
	localtime_s(&local, &now);

	static char time_stamp[16];
	sprintf_s(time_stamp, 15, "%02d:%02d:%02d ", local.tm_hour, local.tm_min, local.tm_sec);

	m_log		<< time_stamp << type << ": " << str << std::endl;
	std::cout	<< time_stamp << type << ": " << str << std::endl;
}


Logger& Logger::getLogger()
{
	static Logger logger("c:\\ASCOfficeHtmlFile.log");
	return logger;
}

