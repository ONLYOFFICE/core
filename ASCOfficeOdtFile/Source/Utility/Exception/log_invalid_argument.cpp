
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "log_invalid_argument.h"
#include "./../Log.h"


log_invalid_argument::log_invalid_argument(const std::string& message)
	: std::invalid_argument(message)
{
	Log::error(message);
}
