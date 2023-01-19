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
#include <boost/algorithm/string.hpp>

#include "calcext_type.h"

#include <ostream>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const calcext_type & _Val)
{
    switch(_Val.get_type())
    {
    case calcext_type::Percent:
        _Wostream << L"percent";
		break;
	case calcext_type::Number:
        _Wostream << L"number";
        break;
    case calcext_type::AutoMaximum:
        _Wostream << L"auto-maximum";
        break;
    case calcext_type::AutoMinimum:
        _Wostream << L"auto-minimum";
        break;
    case calcext_type::Maximum:
        _Wostream << L"maximum";
        break;
    case calcext_type::Minimum:
        _Wostream << L"minimum";
		break;
	case calcext_type::Percentile:
        _Wostream << L"percentile";
        break;
    default:
        break;
    }
    return _Wostream;    
}

calcext_type calcext_type::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (tmp == L"auto-maximum")
        return calcext_type( AutoMaximum );
    else if (tmp == L"auto-minimum")
        return calcext_type( AutoMinimum );
    else if (tmp == L"number")
        return calcext_type( Number );
    else if (tmp == L"percent")
        return calcext_type( Percent );
    else if (tmp == L"maximum")
        return calcext_type( Maximum );
    else if (tmp == L"minimum")
        return calcext_type( Minimum );
    else if (tmp == L"formula")
        return calcext_type( Formula );
	else if (tmp == L"percentile")
        return calcext_type( Percentile );
	else
    {
        return calcext_type( Number );
    }
}

} }
