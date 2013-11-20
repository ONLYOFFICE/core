#pragma once
#ifndef LOG_NOT_IMPLEMENT_INCLUDE_H_
#define LOG_NOT_IMPLEMENT_INCLUDE_H_

#include <stdexcept>
#include <string>


class log_not_implement : public std::runtime_error
{
public:
	explicit log_not_implement(const std::string& message);
};

#endif // LOG_NOT_IMPLEMENT_INCLUDE_H_