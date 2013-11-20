#pragma once
#ifndef LOG_NOT_FIND_INCLUDE_H_
#define LOG_NOT_FIND_INCLUDE_H_

#include "log_runtime_error.h"
#include <string>


class log_not_find : public log_runtime_error
{
public:
	log_not_find(const std::string& what, const std::string& where);
};

#endif // LOG_NOT_FIND_INCLUDE_H_