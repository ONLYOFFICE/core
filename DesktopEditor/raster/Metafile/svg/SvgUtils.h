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

#ifndef SVGUTILS_H
#define SVGUTILS_H

#include <algorithm>
#include <string>
#include <vector>
#include <regex>
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

		inline bool ReadDoubleValue(const std::wstring& wsValue, double& dValue)
		{
			std::wregex oPattern(LR"([-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)");
			std::wsmatch oMatch;

			if (!std::regex_search(wsValue, oMatch, oPattern))
				return false;

			dValue = std::stod(oMatch[0].str());

			return true;
		}

		inline bool ReadAngle(const std::wstring& wsValue, double& dValue)
		{
			std::wregex oPattern(LR"(([-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)(deg|rad|grad|turn))");
			std::wsmatch oMatch;

			if (!std::regex_search(wsValue, oMatch, oPattern))
				return false;

			const std::wstring wsUnit{oMatch[5].str()};

			dValue = std::stod(oMatch[1].str());

			if (L"grad" == wsUnit)
				dValue *= 400. / 360.;
			else if (L"rad" == wsUnit)
				dValue *= 180. / 3.14159;
			else if (L"turn" == wsUnit)
				dValue *= 360.;

			return true;
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
