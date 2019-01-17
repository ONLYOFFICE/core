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

#include "mathvariant.h"
#include <vector>

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const math_variant & math_variant_)
{
    if (math_variant_.is_none())
    {
        return _Wostream;
    }

//.....
    return _Wostream;
}

bool operator == (math_variant & _b1, math_variant & _b2)
{
    if (_b1.is_none() && _b2.is_none()) return true;

    if (_b1.is_none() || _b2.is_none()) return false;

	if (    _b1.style_.bold     == _b2.style_.bold &&
			_b1.style_.italic	== _b2.style_.italic  &&
			_b1.style_.type     == _b2.style_.type)     return true;

    return false;
}


math_variant::math_variant(const math_variant & Value)
{
    style_  = Value.style_;

	none_ = Value.is_none();

}
math_variant& math_variant::operator =(const math_variant& Value)
{
    style_  = Value.style_;

    none_   = Value.is_none();

    return *this;
}
math_variant::math_variant(const std::wstring & Value) :  none_(true)
{
	std::wstring tmp =  boost::algorithm::trim_copy(Value);
    if (tmp.length() < 1) return;
   
	boost::algorithm::to_lower(tmp);
    
	std::vector< std::wstring > splitted;
	boost::algorithm::split(splitted, Value, boost::algorithm::is_any_of(L" \t"), boost::algorithm::token_compress_on);

	for (size_t i = 0 ; i < splitted.size(); i++)
	{
		if (splitted[i] == L"bold")
		{
			style_.bold = true;
		}
		else if (splitted[i] == L"italic")
		{
			style_.italic = true;
		}
		else if (splitted[i] == L"bold-italic")
		{
			style_.bold = true;
			style_.italic = true;
		}		
 		else if (splitted[i] == L"double-struck")
		{
			style_.type = 1;
		}
 		else if (splitted[i] == L"bold-fraktur")
		{
			style_.type = 2;
			style_.bold = true;
		}
  		else if (splitted[i] == L"fraktur")
		{
			style_.type = 2;
		}
		else if (splitted[i] == L"script")
		{
			style_.type = 3;
		}	
 		else if (splitted[i] == L"bold-script")
		{
			style_.type = 3;
			style_.bold = true;
		}	
 		else if (splitted[i] == L"sans-serif-italic")
		{
			style_.type = 4;
			style_.italic = true;
		}
 		else if (splitted[i] == L"bold-sans-serif")
		{
			style_.type = 4;
			style_.bold = true;
		}
 		else if (splitted[i] == L"sans-serif-bold-italic")
		{
			style_.type = 4;
			style_.italic = true;
			style_.bold = true;
		}
 		else if (splitted[i] == L"monospace")
		{
			style_.type = 5;
		}
		else if (splitted[i] == L"stretched")
		{
			//style_.stretched = true;
		}
 		else if (splitted[i] == L"tailed")
		{
			//style_.type = 7;
		}
 		else if (splitted[i] == L"looped")
		{
			//style_.type = 8;
		}
 		else if (splitted[i] == L"initial")
		{
			//style_.type = 9;
		}
	}

	none_ = false;
}

math_variant math_variant::parse( const std::wstring & Value)
{
    return math_variant(Value);

}




}
}
