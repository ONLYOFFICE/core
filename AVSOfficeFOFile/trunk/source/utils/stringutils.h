#pragma once
#include <string>

namespace AVS
{
	namespace Utils
	{
		std::wstring ExtractFileName(const std::wstring & inputString);
		std::wstring ExtractFileNameWithoutExt(const std::wstring & inputString);
		std::wstring ExtractFilePath(const std::wstring & inputString);
	}
}