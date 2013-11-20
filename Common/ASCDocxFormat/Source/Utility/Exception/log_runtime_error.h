#pragma once
#ifndef LOG_RUNTIME_ERROR_INCLUDE_H_
#define LOG_RUNTIME_ERROR_INCLUDE_H_

#include <stdexcept>
#include <string>


class log_runtime_error : public std::runtime_error
{
public:
	explicit log_runtime_error(const std::string& message);
};

#endif // LOG_RUNTIME_ERROR_INCLUDE_H_