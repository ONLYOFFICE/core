#pragma once
#ifndef LOG_NOT_INIT_NULLABLE_INCLUDE_H_
#define LOG_NOT_INIT_NULLABLE_INCLUDE_H_

#include <stdexcept>
#include <string>


class log_not_init_nullable : public std::runtime_error
{
public:
	explicit log_not_init_nullable();
	explicit log_not_init_nullable(const std::string& message);
};

#endif // LOG_NOT_INIT_NULLABLE_INCLUDE_H_