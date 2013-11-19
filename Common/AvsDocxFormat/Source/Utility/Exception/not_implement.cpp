
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "not_implement.h"


not_implement::not_implement()
	: std::runtime_error("")
{
}


not_implement::not_implement(const std::string& message)
	: std::runtime_error(message)
{
}