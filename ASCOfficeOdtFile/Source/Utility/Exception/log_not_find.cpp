
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "log_not_find.h"


log_not_find::log_not_find(const std::string& what, const std::string& where)
: log_runtime_error("not find " + what + " in " + where)
{
}