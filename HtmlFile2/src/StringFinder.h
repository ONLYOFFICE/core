/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#ifndef STRINGFINDER_H
#define STRINGFINDER_H

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <vector>

#if defined(_WIN32) || defined (_WIN64)
#include <tchar.h>
#elif __linux__ || MAC
#include <libgen.h>
#endif

namespace NSStringFinder 
{
	template<class CharType, class StringType = std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>>
	struct TFoundedData
	{
		size_t     m_unBeginPosition;
		size_t     m_unEndPosition;
		StringType m_sValue;

		TFoundedData()
			: m_unBeginPosition(0), m_unEndPosition(0)
		{}

		bool Empty() const
		{
			return 0 == m_unEndPosition || StringType::npos == m_unEndPosition;
		}
	};

	template<class CharType, class StringType = std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>>
	StringType FindPropertyTemplate(const StringType& sString, const StringType& sProperty, const StringType& sDelimiter, const StringType& sEnding, const size_t& unStarting, size_t& unEndPosition)
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

	inline std::string FindProperty(const std::string& sString, const std::string& sProperty, const std::string& sDelimiter, const std::string& sEnding, const size_t& unStarting, size_t& unEndPosition)
	{
		return FindPropertyTemplate<char>(sString, sProperty, sDelimiter, sEnding, unStarting, unEndPosition);
	}

	inline std::wstring FindProperty(const std::wstring& wsString, const std::wstring& wsProperty, const std::wstring& wsDelimiter, const std::wstring& wsEnding, const size_t& unStarting, size_t& unEndPosition)
	{
		return FindPropertyTemplate<wchar_t>(wsString, wsProperty, wsDelimiter, wsEnding, unStarting, unEndPosition);
	}

	inline std::string FindProperty(const std::string& sString, const std::string& sProperty, const std::string& sDelimiter, const std::string& sEnding, const size_t& unStarting = 0)
	{
		size_t unEndPosition = 0;
		return FindPropertyTemplate<char>(sString, sProperty, sDelimiter, sEnding, unStarting, unEndPosition);
	}

	inline std::wstring FindProperty(const std::wstring& wsString, const std::wstring& wsProperty, const std::wstring& wsDelimiter, const std::wstring& wsEnding, const size_t& unStarting = 0)
	{
		size_t unEndPosition = 0;
		return FindPropertyTemplate<wchar_t>(wsString, wsProperty, wsDelimiter, wsEnding, unStarting, unEndPosition);
	}

	template<class CharType, class StringType = std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>>
	TFoundedData<CharType> FindPropertyTemplate(const StringType& sString, const StringType& sProperty, const std::vector<StringType>& arDelimiters, const std::vector<StringType>& arEndings, const size_t& unStarting)
	{
		if (sString.length() < unStarting)
			return TFoundedData<CharType>();

		std::wstring wsRegexValue = L"(?i)" + std::wstring(sProperty.begin(), sProperty.end());

		if (!arDelimiters.empty())
		{
			wsRegexValue += L"\\s*[";
			for (const StringType& wsDelimiter : arDelimiters)
				wsRegexValue +=  std::wstring(wsDelimiter.begin(), wsDelimiter.end()) + L"|";
			wsRegexValue.pop_back();
			wsRegexValue += L"]{1}";
		}

		if (!arEndings.empty())
		{
			std::wstring wsEndingValue;

			for (const StringType& sEnding : arEndings)
				wsEndingValue +=  std::wstring(sEnding.begin(), sEnding.end()) + L"|";

			wsEndingValue.pop_back();

			wsRegexValue += L"\\s*[" + wsEndingValue + L"]?(.[^" + wsEndingValue + L"]*)\\s*[" + wsEndingValue + L"]?";
		}
		else
			wsRegexValue += L"\\s*(.*)[\\n|\\r]?";

		boost::wregex oRegex(wsRegexValue);
		boost::match_results<typename StringType::const_iterator> oResult;

		if (!boost::regex_search(sString.begin() + unStarting, sString.end(), oResult, oRegex))
			return TFoundedData<CharType>();

		TFoundedData<CharType> oData;

		oData.m_unBeginPosition = unStarting + oResult.position();
		oData.m_unEndPosition = unStarting + oResult.position() + oResult.length();

		oData.m_sValue = oResult[1];
		boost::algorithm::trim(oData.m_sValue);

		return oData;
	}

	inline TFoundedData<char> FindProperty(const std::string& sString, const std::string& sProperty, const std::vector<std::string>& arDelimiters, const std::vector<std::string>& arEndings, const size_t& unStarting = 0)
	{
		return FindPropertyTemplate<char>(sString, sProperty, arDelimiters, arEndings, unStarting);
	}

	inline TFoundedData<wchar_t> FindProperty(const std::wstring& wsString, const std::wstring& wsProperty, const std::vector<std::wstring>& arDelimiters, const std::vector<std::wstring>& arEndings, const size_t& unStarting = 0)
	{
		return FindPropertyTemplate<wchar_t>(wsString, wsProperty, arDelimiters, arEndings, unStarting);
	}

	template<class CharType, class StringType = std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>>
	bool RemoveEmptyTagTemplate(StringType& sValue, const StringType& sTagName, size_t unStart)
	{
		boost::wregex oRegex(L"<\\s*(?i)" + std::wstring(sTagName.begin(), sTagName.end()) + L"\\s*[^>]*/>");
		boost::match_results<typename StringType::const_iterator> oResult;

		if (unStart >= sValue.length() || !boost::regex_search(sValue.cbegin() + unStart, sValue.cend(), oResult, oRegex))
			return false;

		sValue.erase(unStart + oResult.position(), oResult.length());

		return true;
	}

	inline bool RemoveEmptyTag(std::string& sValue, const std::string& sTagName, size_t unStart = 0)
	{
		return RemoveEmptyTagTemplate<char>(sValue, sTagName, unStart);
	}

	inline bool RemoveEmptyTag(std::wstring& sValue, const std::wstring& sTagName, size_t unStart = 0)
	{
		return RemoveEmptyTagTemplate<wchar_t>(sValue, sTagName, unStart);
	}

	template <typename StringType, typename StringEndgeType>
	void CutInside(StringType& sString, const StringEndgeType& sLeftEdge, const StringEndgeType& sRightEdge)
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

	template <typename StringType, typename StringEdgeType>
	void CutInside(StringType& sString, const StringEdgeType& sEdge)
	{
		CutInside(sString, sEdge, sEdge);
	}

	template <typename StringFirstType, typename StringSecondType>
	bool Equals(const StringFirstType& sFirstString, const StringSecondType& sSecondString)
	{
		return boost::iequals(sFirstString, sSecondString);
	}

	template <typename StringFirstType, typename StringSecondType>
	bool EqualOf(const StringFirstType& sFirstString, const std::vector<StringSecondType>& arStrings)
	{
		for (const StringFirstType& sString : arStrings)
			if (boost::iequals(sFirstString, sString))
				return true;

		return false;
	}

	template <typename StringFirstType, typename StringSecondType>
	bool EqualOf(const StringFirstType& sFirstString, const std::initializer_list<StringSecondType>& arStrings)
	{
		for (const StringFirstType& sString : arStrings)
			if (boost::iequals(sFirstString, sString))
				return true;

		return false;
	}

	template <typename StringType, typename StringValueType>
	bool Find(const StringType& sString, const StringValueType& sValue)
	{
		return !boost::algorithm::ifind_first(sString, sValue).empty();
	}

	inline int ToInt(const std::wstring& oValue, int nMinValue = 0)
	{
		boost::wregex oRegex(LR"((-?\.\d+|-?\d+(\.\d+)?))");

		boost::match_results<typename std::wstring::const_iterator> oResult;

		if (!boost::regex_search(oValue.begin(), oValue.end(), oResult, oRegex))
			return nMinValue;

		const int nValue = std::stoi(*oResult.begin());

		return std::max(nMinValue, nValue);
	}
	
	inline int ToDouble(const std::wstring& oValue, double dMinValue = 0.)
	{
		boost::wregex oRegex(LR"((-?\.\d+|-?\d+(\.\d+)?))");

		boost::match_results<typename std::wstring::const_iterator> oResult;

		if (!boost::regex_search(oValue.begin(), oValue.end(), oResult, oRegex))
			return dMinValue;

		const double dValue = std::stod(*oResult.begin());

		return (dValue >= dMinValue) ? dValue : dMinValue;
	}
}

#endif // STRINGFINDER_H
