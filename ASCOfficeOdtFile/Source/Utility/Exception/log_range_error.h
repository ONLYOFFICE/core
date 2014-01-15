#pragma once
#ifndef LOG_RANGE_ERROR_INCLUDE_H_
#define LOG_RANGE_ERROR_INCLUDE_H_

#include <stdexcept>
#include <string>


class log_range_error : public std::range_error
{
public:
	explicit log_range_error(const std::string& message);
	explicit log_range_error(const std::wstring& message);
};

#endif // LOG_RANGE_ERROR_INCLUDE_H_