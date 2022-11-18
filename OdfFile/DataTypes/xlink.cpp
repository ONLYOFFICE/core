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

#include "xlink.h"
#include <boost/algorithm/string.hpp>

#include <ostream>

namespace cpdoccore {
namespace odf_types {

std::wostream & operator << (std::wostream & _Wostream, const xlink_type & _Type)
{
    switch (_Type.get_type())
    {
    default:
    case xlink_type::Simple:
        _Wostream << L"simple";
        break;
    }
    return _Wostream;
}

std::wostream & operator << (std::wostream & _Wostream, const xlink_actuate & _Actuate)
{
    switch (_Actuate.get_type())
    {
    case xlink_actuate::OnLoad:
        _Wostream << "onLoad";   
		break;
	default:
    case xlink_actuate::OnRequest:
        _Wostream << "onRequest";
        break;
    }
    return _Wostream;
}

std::wostream & operator << (std::wostream & _Wostream, const xlink_show & _Show)
{
    switch (_Show.get_type())
    {
    case xlink_show::Replace:
        _Wostream << L"replace";
        break;
     case xlink_show::Embed:
        _Wostream << L"embed";
        break;
	default:
    case xlink_show::New:
        _Wostream << L"new";
        break;
    }
    return _Wostream;
}

xlink_type xlink_type::parse(const std::wstring & Str)
{
	return xlink_type( Simple );
}

xlink_actuate xlink_actuate::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

	if (tmp == L"onrequest")
        return xlink_actuate( OnRequest );
    else if (tmp == L"onload")
        return xlink_actuate( OnLoad );   
	else
    {
        return xlink_actuate( OnRequest );
    }
}

xlink_show xlink_show::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"new")
        return xlink_show( New );
    else if (tmp == L"replace")
        return xlink_show( Replace );
     else if (tmp == L"embed")
        return xlink_show( Embed );
	else
    {
        return xlink_show( New );
    }
}

}
}
