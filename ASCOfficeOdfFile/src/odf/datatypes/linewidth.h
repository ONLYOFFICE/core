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
#ifndef _CPDOCCORE_ODF_LINEWIDTH_H_
#define _CPDOCCORE_ODF_LINEWIDTH_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>
#include <string>
#include "odfattributes.h"
#include "lengthorpercent.h"



namespace cpdoccore { namespace odf_types { 

class line_width
{
public:
    enum type
    {
        Auto,
        Normal,
        Bold,
        Thin,
        Dash,
        Medium,
        Thick,

        PositiveInteger,
        Percent,
        PositiveLength
    };

    line_width() {}

    line_width(type _Type) : type_(_Type)
    {}

    line_width(unsigned int _Val) : type_(PositiveInteger), PositiveInteger_(_Val)
    {}
    line_width(length_or_percent _Val) ;
    line_width(percent _Percent) : type_(Percent), Percent_(_Percent)
    {}

    line_width(length _Length) : type_(PositiveLength), PositiveLength_(_Length)
    {}

    type get_type() const 
    {
        return type_;
    }
    
    unsigned int get_positive_integer() const 
    {
        return PositiveInteger_;
    }

    const percent & get_percent() const 
    {
        return Percent_;        
    }

    const length & get_positive_length() const 
    {
        return PositiveLength_;
    }

    static line_width parse(const std::wstring & );

private:
    type type_;
    unsigned int PositiveInteger_;
    percent Percent_;
    length PositiveLength_;
};

std::wostream & operator << (std::wostream & _Wostream, const line_width & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::line_width);

}

#endif
