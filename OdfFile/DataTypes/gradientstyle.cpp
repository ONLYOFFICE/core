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

#include "gradientstyle.h"
#include <boost/algorithm/string.hpp>
#include <ostream>

namespace cpdoccore { 
namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const gradient_style & _Val)
{
    switch(_Val.get_type())
    {
	case gradient_style::linear:
        _Wostream << L"linear";
        break;
	case gradient_style::axial:
        _Wostream << L"axial";
        break;
 	case gradient_style::radial:
        _Wostream << L"radial";
        break;
  	case gradient_style::ellipsoid:
        _Wostream << L"ellipsoid";
        break;
  	case gradient_style::square:
        _Wostream << L"square";
        break;
  	case gradient_style::rectangular:
        _Wostream << L"rectangular";
        break;
  }
    return _Wostream;
}

gradient_style gradient_style::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"linear")
        return gradient_style( linear );
    else if (tmp == L"axial")
        return gradient_style( axial );
    else if (tmp == L"radial")
        return gradient_style(radial );
    else if (tmp == L"ellipsoid")
        return gradient_style( ellipsoid);
    else if (tmp == L"square")
        return gradient_style( square );
    else if (tmp == L"rectangular")
        return gradient_style( rectangular );   
	else
    {
        return gradient_style( linear );
    }
}
}
}
