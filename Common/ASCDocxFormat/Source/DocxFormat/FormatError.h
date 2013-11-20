#pragma once
#ifndef OOX_FORMAT_ERROR_INCLUDE_H_
#define OOX_FORMAT_ERROR_INCLUDE_H_

#include <stdexcept>
#include <string>


namespace OOX
{
	class FormatError : public std::runtime_error
	{
	public:
		explicit FormatError(const std::string& message);
		explicit FormatError(const std::wstring& message);
	};
} // namesapce OOX

#endif // OOX_FORMAT_ERROR_INCLUDE_H_