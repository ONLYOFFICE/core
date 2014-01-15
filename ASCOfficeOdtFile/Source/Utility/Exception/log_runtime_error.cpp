
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "log_runtime_error.h"
#include "./../Log.h"


log_runtime_error::log_runtime_error(const std::string& message)
	: std::runtime_error(message)
{
	Log::error(message);
}
