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

#include "linewidth.h"
#include "../Common/errors.h"

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const line_width & _Val)
{
    switch(_Val.get_type())
    {
    case line_width::Auto:
        _Wostream << "auto";
        break;
    case line_width::Normal:
        _Wostream << "normal";
        break;
    case line_width::Bold:
        _Wostream << "bold";
        break;
    case line_width::Thin:
        _Wostream << "thin";
        break;
    case line_width::Dash:
        _Wostream << "dash";
        break;
    case line_width::Medium:
        _Wostream << "medium";
        break;
    case line_width::Thick:
        _Wostream << "thick";
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

