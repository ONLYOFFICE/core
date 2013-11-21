#pragma once

#include <stdexcept>

namespace EXCEPT
{;


class LogicalException : public std::logic_error
{
protected:
	explicit LogicalException(const std::string& message);
};


class RuntimeException : public std::runtime_error
{
protected:
	explicit RuntimeException(const std::string& message);
};

class StopConversionException: public std::runtime_error
{
protected:
	explicit StopConversionException(const std::string& message);
};

} // namespace EXCEPT

