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

#include "percent.h"
#include <iostream>
#include <sstream>
#include "../Common/errors.h"

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const percent & _Percent)
{
    _Wostream << _Percent.get_value() << L"%";
    return _Wostream;
}

std::wistream & operator >> (std::wistream & _Wistream, percent & _Percent)
{
    wchar_t p;
    double val;
    _Wistream >> val >> p;
    if (p != L'%')
    {
        BOOST_THROW_EXCEPTION( errors::invalid_attribute() );    
    }

    _Percent = percent(val);
    return _Wistream;
}

percent percent::parse(const std::wstring & Str)
{
    percent per_;
    std::wstringstream strm;
    strm << Str;
    strm >> per_;
    return per_;
}
//----------------------------------------------------------------

std::wostream & operator << (std::wostream & _Wostream, const percent_rel & _percent_rel)
{
	if (_percent_rel.get_value()  >= 0)
		_Wostream << _percent_rel.get_value() << L"*";
	else 
		_Wostream << L"-1*";
    return _Wostream;
}

std::wistream & operator >> (std::wistream & _Wistream, percent_rel & _percent_rel)
{
    wchar_t p;
    int val;
    _Wistream >> val >> p;
   
	if (p != L'*')
        _percent_rel = percent_rel(0);   
	else
		_percent_rel = percent_rel(val);
    return _Wistream;
}

percent_rel percent_rel::parse(const std::wstring & Str)
{
    percent_rel per_(0);
    std::wstringstream strm;
    strm << Str;
    strm >> per_;
    return per_;
}


}
}

