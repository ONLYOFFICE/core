
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "log_range_error.h"
#include "./../Log.h"


log_range_error::log_range_error(const std::string& message)
	: std::range_error(message)
{
	Log::error(message);
}


log_range_error::log_range_error(const std::wstring& message)
	: std::range_error("")
{
	Log::error(message);
}