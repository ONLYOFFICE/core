#ifndef SVGUTILS_H
#define SVGUTILS_H

#include <algorithm>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <cfloat>

namespace SVG
{
	enum Metrics
	{
		EM,
		EX,
		PX,
		PT,
		PC,
		CM,
		MM,
		INCH,

		PCT,	//	percent

		UNDEFINED
	};

	inline bool Equals(double dFirst, double dSecond, double dEpsilon = DBL_EPSILON)
	{
		return std::abs(dFirst - dSecond) <= dEpsilon;
	}

	namespace StrUtils
	{
		inline std::vector<double> ReadDoubleValues(std::wstring::const_iterator oBegin, std::wstring::const_iterator oEnd)
		{
			std::vector<double> arValues;

			std::wregex oPattern(LR"([-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)");

			std::wsregex_iterator oIter(oBegin, oEnd, oPattern);
			std::wsregex_iterator oEndIter;

			for (; oIter != oEndIter; ++oIter)
				arValues.push_back(std::stod(oIter->str()));

			return arValues;
		}

		inline std::vector<double> ReadDoubleValues(const std::wstring& wsValue)
		{
			return ReadDoubleValues(wsValue.begin(), wsValue.end());
		}

		inline std::wstring TrimExtraEnding(const std::wstring& wsString)
		{
			std::wstring::const_iterator itBeginPos = std::find_if(wsString.begin(), wsString.end(), [](const wchar_t& wChar){ return !iswspace(wChar);});
			std::wstring::const_reverse_iterator itEndPos =  std::find_if(wsString.rbegin(), wsString.rend(), [](const wchar_t& wChar){ return !iswspace(wChar);});

			if (wsString.end() == itBeginPos)
				return std::wstring();

			return std::wstring(itBeginPos, itEndPos.base()) + ((wsString.end() != itEndPos.base()) ? L" " : L"");
		}
	};
}

#endif // SVGUTILS_H
