#pragma once

#include "Logger.h"

namespace Log
{
	void info(const std::string& message);
	void event(const std::string& message);
	void message(const std::string& message);
	void warning(const std::string& message);
	void error(const std::string& message);

}; // namespace Log


