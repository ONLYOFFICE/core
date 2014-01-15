
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "Log.h"
#include <fstream>
#include <iostream>


Details::Log Log::s_loger("converter.log", "converter.err");


void Log::event(const std::string& message)
{
	s_loger.event(message);
}


void Log::event(const std::wstring& message)
{
	s_loger.event(message);
}


void Log::warning(const std::string& message)
{
	s_loger.warning(message);
}


void Log::warning(const std::wstring& message)
{
	s_loger.warning(message);
}


void Log::error(const std::string& message)
{
	s_loger.error(message);
}


void Log::error(const std::wstring& message)
{
	s_loger.error(message);
}


void Log::message(const std::string& message)
{
	s_loger.message(message);
}


void Log::message(const std::wstring& message)
{
	s_loger.message(message);
}