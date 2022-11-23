/*
* (c) Copyright Ascensio System SIA 2010-2019
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
* You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

#include "drawangle.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore {
	namespace odf_types {

		std::wostream & operator << (std::wostream & _Wostream, const draw_angle & _Val)
		{
			_Wostream << (int)(_Val.get_value()) << L"deg"; //?? или округлть до 2 знаков???
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
				//старая версия содержит множитель 10.
			}

			int val = (value * 100);
			val = val % 360000;
			if (val < 0) val += 360000;

			value = val / 1000.;			

			return draw_angle(value);
		}
	}
}
