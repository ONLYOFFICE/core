/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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
