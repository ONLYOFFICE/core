#include "precompiled_xls.h"
#include "Log.h"


namespace Log
{

void info(const std::string& message)
{
#ifdef _DEBUG
	Logger::getLogger().writeLine("Info", message);
#endif
}


void event(const std::string& message)
{
#ifdef _DEBUG
	Logger::getLogger().writeLine("Event", message);
#endif
}


void message(const std::string& message)
{
#ifdef _DEBUG
	Logger::getLogger().writeLine("Message", message);
#endif
}


void warning(const std::string& message)
{
#ifdef _DEBUG
	Logger::getLogger().writeLine("WARNING", message);
#endif
}
void warning(const std::wstring& message)
{
#ifdef _DEBUG
	Logger::getLogger().writeLine(L"WARNING", message);
#endif
}


void error(const std::string& message)
{
#ifdef _DEBUG
	Logger::getLogger().writeLine("!!ERROR", message);
#endif
}


}; // namespace Log
