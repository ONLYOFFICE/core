/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once


#include <iosfwd>
#include <string>
#include "odfattributes.h"
#include "length.h"


namespace cpdoccore { namespace odf_types { 

class border_widths
{
public:
    border_widths() {}

    border_widths(length _Len1, length _Len2, length _Len3) : len1_(_Len1), len2_(_Len2), len3_(_Len3)
    {}
    
    static border_widths parse(const std::wstring & Str);

    const length & get_len1() const
    {
        return len1_;
    }
    
    const length & get_len2() const
    {
        return len2_;
    }

    const length & get_len3() const
    {
        return len3_;
    }

    double get_summ_unit(length::unit Unit) const
    {
        return len1_.get_value_unit(Unit) + len2_.get_value_unit(Unit) + len3_.get_value_unit(Unit);
    }

private:
    length len1_;
    length len2_;
    length len3_;

};

std::wostream & operator << (std::wostream & _Wostream, const border_widths & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::border_widths);

}
