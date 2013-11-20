
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "log_not_init_nullable.h"
#include "./../Log.h"


log_not_init_nullable::log_not_init_nullable()
	: std::runtime_error("not_init_nullable")
{
	Log::error("not_init_nullable");
}


log_not_init_nullable::log_not_init_nullable(const std::string& message)
	: std::runtime_error(message)
{
	Log::error(message);
}