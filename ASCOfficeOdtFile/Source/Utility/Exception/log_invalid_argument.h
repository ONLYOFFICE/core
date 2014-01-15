#pragma once
#ifndef LOG_INVALID_ARGUMENT_INCLUDE_H_
#define LOG_INVALID_ARGUMENT_INCLUDE_H_

#include <stdexcept>
#include <string>


class log_invalid_argument : public std::invalid_argument
{
public:
	explicit log_invalid_argument(const std::string& message);
};

#endif // LOG_INVALID_ARGUMENT_INCLUDE_H_