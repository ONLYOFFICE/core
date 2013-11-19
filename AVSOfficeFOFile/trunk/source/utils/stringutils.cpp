#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>

namespace AVS
{
	namespace Utils
	{
		std::wstring ExtractFileName(const std::wstring & inputString)
		{
			std::vector<std::wstring> result;
			boost::algorithm::split(result, inputString, boost::algorithm::is_any_of(L"\\/"));
			if (result.size() > 0)
				return result[result.size() - 1];
			else
				return L"";
		}
		std::wstring ExtractFileNameWithoutExt(const std::wstring & inputString)
		{
			std::vector<std::wstring> result;
			boost::algorithm::split(result, ExtractFileName(inputString), boost::algorithm::is_any_of(L"."));
			return result[result.size() - 1];
		}
		
		std::wstring ExtractFilePath(const std::wstring & inputString)
		{
			std::vector<std::wstring> result;
			boost::algorithm::split(result, inputString, boost::algorithm::is_any_of(L"\\/"));
			result.pop_back();
			return boost::algorithm::join(result, std::wstring(L"\\"));
		}
	}
}