
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "log_not_implement.h"
#include "./../Log.h"


log_not_implement::log_not_implement(const std::string& message)
	: std::runtime_error(message)
{
	Log::error(message);
}