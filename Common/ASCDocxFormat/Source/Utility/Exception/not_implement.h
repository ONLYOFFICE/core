#pragma once
#ifndef NOT_IMPLEMENT_INCLUDE_H_
#define NOT_IMPLEMENT_INCLUDE_H_

#include <stdexcept>
#include <string>


class not_implement : public std::runtime_error
{
public:
	not_implement();
	explicit not_implement(const std::string& message);
};

#endif // NOT_IMPLEMENT_INCLUDE_H_