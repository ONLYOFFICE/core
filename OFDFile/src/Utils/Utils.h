#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cfloat>
#include <cctype>
#include <cmath>

#include "../../../DesktopEditor/common/Path.h"
#include "../../../DesktopEditor/common/ProcessEnv.h"

#include <boost/algorithm/string/predicate.hpp>

namespace OFD
{
inline std::vector<std::string> Split(const std::string& sValue, char chDelim)
{
	if (sValue.empty())
		return std::vector<std::string>();

	std::vector<std::string> arValues;
	std::stringstream oStringStream(sValue);
	std::string sItem;

	while (std::getline(oStringStream, sItem, chDelim))
		arValues.push_back(sItem);

	return arValues;
}

inline bool StringToBoolean(const std::string& sValue, bool& bValue)
{
	//TODO:: скорректировать метод, если возможно чтение не стандартных значений с плавающей запятой
	if (sValue.empty())
		return false;

	size_t unStart = sValue.find_first_not_of(" \t\n\r\f\v");

	if (std::string::npos == unStart)
		return false;

	size_t unEnd = sValue.find_last_not_of(" \t\n\r\f\v");

	std::string sTrimmed = sValue.substr(unStart, unEnd - unStart + 1);

	std::transform(sTrimmed.begin(), sTrimmed.end(), sTrimmed.begin(),
				   [](unsigned char c){ return std::tolower(c); });

	bValue = "true" == sTrimmed;
	return true;
}

inline bool StringToInteger(const std::string& sValue, int& nValue)
{
	//TODO:: скорректировать метод, если возможно чтение не стандартных значений с плавающей запятой
	if (sValue.empty())
		return false;

	char* pEnd = nullptr;

	nValue = std::strtol(sValue.c_str(), &pEnd, 10);

	if (pEnd == sValue.c_str() || '\0' != *pEnd)
		return false;

	return true;
}

inline bool StringToUInteger(const std::string& sValue, unsigned int& unValue)
{
	//TODO:: скорректировать метод, если возможно чтение не стандартных значений с плавающей запятой
	if (sValue.empty())
		return false;

	char* pEnd = nullptr;

	unValue = std::strtoul(sValue.c_str(), &pEnd, 10);

	if (pEnd == sValue.c_str() || '\0' != *pEnd)
		return false;

	return true;
}

inline bool StringToDouble(const std::string& sValue, double& dValue)
{
	//TODO:: скорректировать метод, если возможно чтение не стандартных значений с плавающей запятой
	if (sValue.empty())
		return false;

	char* pEnd = nullptr;

	dValue = std::strtod(sValue.c_str(), &pEnd);

	if (pEnd == sValue.c_str() || '\0' != *pEnd)
		return false;

	return true;
}

template <typename T>
inline void AddToContainer(T* pValue, std::vector<T*>& arValues)
{
	if (nullptr != pValue)
		arValues.push_back(pValue);
}

template <typename T>
inline void ClearContainer(std::vector<T*>& arValues)
{
	for (T* pElement : arValues)
		if (nullptr != pElement)
			delete pElement;

	arValues.clear();
}

inline std::wstring CombinePaths(const std::wstring& wsFirstPath, const std::wstring& wsSecondPath)
{
	if (wsFirstPath.empty())
		return wsSecondPath;

	if (wsSecondPath.empty())
		return wsFirstPath;

	std::wstring::const_reverse_iterator itFound = std::find_if(wsFirstPath.crbegin(), wsFirstPath.crend(), [](wchar_t wChar){ return L'/' == wChar || L'\\' == wChar; });

	if (wsFirstPath.crend() == itFound)
		return NSSystemPath::Combine(wsFirstPath, wsSecondPath);

	std::wstring wsNewSecondPath{wsSecondPath};

	if (L'/' == wsNewSecondPath[0] || L'\\' == wsNewSecondPath[0])
		wsNewSecondPath.erase(0, 1);

	const std::wstring& wsFirstDirName = wsFirstPath.substr(itFound.base() - wsFirstPath.cbegin(), wsFirstPath.cend() - itFound.base());

	if (wsFirstDirName == wsNewSecondPath.substr(0, wsFirstDirName.length()))
		wsNewSecondPath.erase(0, wsFirstDirName.length());

	return NSSystemPath::Combine(wsFirstPath, wsNewSecondPath);
}

inline bool GetStatusUsingExternalLocalFiles()
{
	if (NSProcessEnv::IsPresent(NSProcessEnv::Converter::gc_allowPrivateIP))
		return NSProcessEnv::GetBoolValue(NSProcessEnv::Converter::gc_allowPrivateIP);

	return true;
}

inline bool CanUseThisPath(const std::wstring& wsPath, const std::wstring& wsRootPath)
{
	if (GetStatusUsingExternalLocalFiles())
		return true;

	const std::wstring wsFullPath = NSSystemPath::ShortenPath(NSSystemPath::Combine(wsRootPath, wsPath));

	if (!wsRootPath.empty())
		return boost::starts_with(wsFullPath, wsRootPath);

	return !boost::starts_with(wsFullPath, L"../");
}

inline bool IsZeroValue(const double& dValue)
{
	return DBL_EPSILON > std::fabs(dValue);
}
}
#endif // UTILS_H
