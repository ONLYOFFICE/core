#ifndef STRINGFINDER_H
#define STRINGFINDER_H

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <vector>

namespace NSStringFinder 
{
	template <typename StringType>
	StringType FindPropety(const StringType& sString, const StringType& sProperty, const StringType& sDelimiter, const StringType& sEnding)
	{
		size_t unEndPosition = 0;
		return FindPropety(sString, sProperty, sDelimiter, sEnding, 0, unEndPosition);
	}
	
	template <typename StringType>
	StringType FindPropety(const StringType& sString, const StringType& sProperty, const StringType& sDelimiter, const StringType& sEnding, const size_t& unStarting, size_t& unEndPosition)
	{
		if (sString.length() < unStarting)
			return StringType();

		typedef const boost::iterator_range<typename StringType::const_iterator> StringRange;

		StringRange itFound = boost::algorithm::ifind_first(StringRange(sString.begin() + unStarting, sString.end()), sProperty);

		if (itFound.empty())
			return StringType();

		StringRange itFoundBegin = boost::algorithm::ifind_first(StringRange(itFound.end(), sString.end()), sDelimiter);

		if (itFoundBegin.empty())
			return StringType();

		StringRange itFoundEnd = boost::algorithm::ifind_first(StringRange(itFoundBegin.end(), sString.end()), sEnding);

		if (itFoundEnd.empty())
			return StringType();

		unEndPosition += (itFoundEnd.end() - sString.begin());

		StringType sValue{itFoundBegin.end(), itFoundEnd.begin()};
		boost::algorithm::trim(sValue);
		return sValue;
	}
	
	template <typename StringType>
	StringType FindPropety(const StringType& sString, const StringType& sProperty, const std::vector<StringType>& arDelimiters, const std::vector<StringType>& arEndings, const size_t& unStarting, size_t& unEndPosition)
	{
		if (sString.length() < unStarting)
			return StringType();

		std::string sRegexValue = "(?i)" + sProperty;

		if (!arDelimiters.empty())
		{
			sRegexValue += "\\s*[";
			for (const StringType& sDelimiter : arDelimiters)
				sRegexValue += sDelimiter + "|";
			sRegexValue.pop_back();
			sRegexValue += "]{1}";
		}

		if (!arEndings.empty())
		{
			std::string sEndingValue;

			for (const StringType& sEnding : arEndings)
				sEndingValue += sEnding + "|";

			sEndingValue.pop_back();

			sRegexValue += "\\s*(.[^" + sEndingValue + "]*)\\s*[" + sEndingValue + "]{1}";
		}
		else
			sRegexValue += "\\s*(.*)[\\n|\\r]{1}";

		boost::regex oRegex(sRegexValue);
		boost::match_results<typename StringType::const_iterator> oResult;

		if (!boost::regex_search(sString.begin() + unStarting, sString.end(), oResult, oRegex))
			return StringType();

		unEndPosition = unStarting + oResult.position() + oResult.length();

		StringType sValue(oResult[1]);
		boost::algorithm::trim(sValue);

		return sValue;
	}

	template <typename StringType>
	StringType FindPropety(const StringType& sString, const StringType& sProperty, const std::vector<StringType>& arDelimiters, const std::vector<StringType>& arEndings, const size_t& unStarting)
	{
		size_t unTempEnding = 0;
		return FindPropety(sString, sProperty, arDelimiters, arEndings, unStarting, unTempEnding);
	}

	template <typename StringType>
	void CutInside(StringType& sString, const StringType& sLeftEdge, const StringType& sRightEdge)
	{
		typedef const boost::iterator_range<typename StringType::const_iterator> StringRange;

		StringRange itFoundBegin = boost::algorithm::ifind_first(StringRange(sString.begin(), sString.end()), sLeftEdge);

		if (itFoundBegin.empty())
			return;

		StringRange itFoundEnd = boost::algorithm::ifind_first(StringRange(itFoundBegin.end(), sString.cend()), sRightEdge);

		if (itFoundEnd.empty())
		{
			sString = StringType{itFoundBegin.end(), sString.cend()};
			return;
		}

		sString = StringType{itFoundBegin.end(), itFoundEnd.begin()};
	}

	template <typename StringType>
	void CutInside(StringType& sString, const StringType& sEdge)
	{
		CutInside(sString, sEdge, sEdge);
	}

	template <typename StringType>
	bool Equals(const StringType& sFirstString, const StringType& sSecondString)
	{
		return boost::iequals(sFirstString, sSecondString);
	}
}

#endif // STRINGFINDER_H
