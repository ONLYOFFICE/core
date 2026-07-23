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

#include "drawangle.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore {
	namespace odf_types {

		std::wostream & operator << (std::wostream & _Wostream, const draw_angle & _Val)
		{
			_Wostream << (int)(_Val.get_value()) << L"deg"; //?? or round to 2 decimal places???
			return _Wostream;
		}

		draw_angle draw_angle::parse(const std::wstring & Str)
		{
			std::wstring tmp = Str;
			boost::algorithm::to_lower(tmp);

			double value = XmlUtils::GetDouble(Str);
			
			if (std::wstring::npos != tmp.find(L"rad"))
			{
				value = (180. * value / 3.14159265358979323846) * 10.;
			}
			else if (std::wstring::npos != tmp.find(L"deg"))
			{
				value *= 10.;
			}
			else if (std::wstring::npos != tmp.find(L"grad"))
			{
				value = (value * 9.0 / 10.0) * 10.0;
			}
			else // no unit
			{
				// old version contains multiplier 10.
			}

			int val = (value * 100);
			val = val % 360000;
			if (val < 0) val += 360000;

			value = val / 1000.;			

			return draw_angle(value);
		}
	}
}
