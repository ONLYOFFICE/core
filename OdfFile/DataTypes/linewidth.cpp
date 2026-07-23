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

#include "linewidth.h"
#include "../Common/errors.h"

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const line_width & _Val)
{
    switch(_Val.get_type())
    {
    case line_width::Auto:
        _Wostream << L"auto";
        break;
    case line_width::Normal:
        _Wostream << L"normal";
        break;
    case line_width::Bold:
        _Wostream << L"bold";
        break;
    case line_width::Thin:
        _Wostream << L"thin";
        break;
    case line_width::Dash:
        _Wostream << L"dash";
        break;
    case line_width::Medium:
        _Wostream << L"medium";
        break;
    case line_width::Thick:
        _Wostream << L"thick";
        break;
    case line_width::PositiveInteger:
        _Wostream << _Val.get_positive_integer();
        break;
    case line_width::Percent:
        _Wostream << _Val.get_percent();
        break;
    case line_width::PositiveLength:
        _Wostream << _Val.get_positive_length();
        break;
    }
    return _Wostream;
}

line_width::line_width(length_or_percent _Val)
{
	if (_Val.get_type() == length_or_percent::Length)
	{
		type_= PositiveLength;
		PositiveLength_ = _Val.get_length();
	}
	if (_Val.get_type() == length_or_percent::Percent)
	{
		type_= Percent;
		Percent_ = _Val.get_percent();
	}	
}

line_width line_width::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

    if (boost::algorithm::all(tmp, boost::algorithm::is_digit()))
    {
        try 
        {
            return line_width( boost::lexical_cast<unsigned int>(tmp) );
        }
        catch(...) {}
    } 

    if (boost::algorithm::contains(tmp, L"%"))
    {
        try 
        {
            return line_width( percent::parse(tmp) );
        }
        catch(errors::invalid_attribute &) {}
    }
    
    if (tmp == L"auto")
        return line_width( Auto );
    else if (tmp == L"normal")
        return line_width( Normal );
    else if (tmp == L"bold")
        return line_width( Bold );
    else if (tmp == L"thin")
        return line_width( Thin );
    else if (tmp == L"dash")
        return line_width( Dash );
    else if (tmp == L"medium")
        return line_width( Medium );
    else if (tmp == L"thick")
        return line_width( Thick );

    try 
    {
        return line_width( length::parse(tmp) );
    } 
    catch(errors::invalid_attribute & ) {}

    return line_width(Auto);
}

} 
}

