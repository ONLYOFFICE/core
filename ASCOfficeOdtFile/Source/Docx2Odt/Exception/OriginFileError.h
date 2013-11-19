#pragma once
#ifndef	OOX_2_ODT_ORIGIN_FILE_INCLUDE_H_
#define OOX_2_ODT_ORIGIN_FILE_INCLUDE_H_

#include <stdexcept>
#include <string>


namespace OOX2Odt
{
	class OriginFileError : public std::runtime_error
	{
	public:
		OriginFileError(const std::string& message);
	};
} // namespace Odt2OOX

#endif // OOX_2_ODT_ORIGIN_FILE_ERROR_INCLUDE_H_