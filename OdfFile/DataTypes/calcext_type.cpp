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
