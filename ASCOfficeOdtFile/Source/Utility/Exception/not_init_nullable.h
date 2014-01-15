#pragma once
#ifndef NOT_INIT_NULLABLE_INCLUDE_H_
#define NOT_INIT_NULLABLE_INCLUDE_H_

#include <stdexcept>
#include <string>


class not_init_nullable : public std::runtime_error
{
public:
	explicit not_init_nullable();
	explicit not_init_nullable(const std::string& message);
};

#endif // NOT_INIT_NULLABLE_INCLUDE_H_