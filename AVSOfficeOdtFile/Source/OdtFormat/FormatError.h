#pragma once
#ifndef ODT_FORMAT_ERROR_INCLUDE_H_
#define ODT_FORMAT_ERROR_INCLUDE_H_

#include <stdexcept>
#include <string>


namespace Odt
{
	class FormatError : public std::runtime_error
	{
	public:
		explicit FormatError(const std::string& message);
	};
} // namespace Odt

#endif // ODT_FORMAT_ERROR_INCLUDE_H_