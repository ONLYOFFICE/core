#include "stdafx.h"

#ifdef ENABLE_LOG

#include "Log.h"
#include <fstream>
#include <iostream>


Details::Log Log::s_loger("converter.log", "converter.err");


void Log::event(const std::string& message)
{
	s_loger.event(message);
}


void Log::warning(const std::string& message)
{
	s_loger.warning(message);
}


void Log::error(const std::string& message)
{
	s_loger.error(message);
}


void Log::message(const std::string& message)
{
	s_loger.message(message);
}

#endif