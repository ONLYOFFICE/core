#pragma once
#ifndef	UNIT_ERROR_INCLUDE_H_
#define UNIT_ERROR_INCLUDE_H_

#include "log_invalid_argument.h"
#include <string>


class UnitError : public log_invalid_argument
{
public:
	UnitError(const std::string& message);
};

#endif // UNIT_ERROR_INCLUDE_H_