#pragma once

#ifdef ENABLE_LOG

#include <string>
#include "LogDetails.h"


class Log
{
public:
	static void event(const std::string& message);
	static void message(const std::string& message);
	static void warning(const std::string& message);
	static void error(const std::string& message);

private:
	static Details::Log s_loger;
};

#endif