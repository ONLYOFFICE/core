
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "not_init_nullable.h"


not_init_nullable::not_init_nullable()
	: std::runtime_error("not_init_nullable")
{
}


not_init_nullable::not_init_nullable(const std::string& message)
	: std::runtime_error(message)
{
}