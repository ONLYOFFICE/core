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
#ifndef _CPDOCCORE_ODF_TEXTPOSITION_H_
#define _CPDOCCORE_ODF_TEXTPOSITION_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <string>
#include <iosfwd>
#include "percent.h"


namespace cpdoccore { namespace odf_types { 

class text_position
{
public:
    enum type 
    {
        Percent,
        Sub,
        Super
    };

public:
    text_position()
    {}

    text_position(type _Type) : type_(_Type), 
        has_font_size_(false)
    {}

    text_position(double _Percent) : type_(Percent), 
        position_(_Percent), 
        has_font_size_(false)
    {}

    text_position(type _Type, double _FontSize) : type_(_Type), 
        has_font_size_(true), 
        font_size_(_FontSize)
    {}

    text_position(double _Percent, double _FontSize) : type_(Percent), 
        position_(_Percent), 
        has_font_size_(true), 
        font_size_(_FontSize)
    {}

    type get_type() const
    {
        return type_;
    }

    const percent & get_position() const
    {
        return position_;
    }

    bool has_font_size() const
    {
        return has_font_size_; 
    }

    const percent & font_size() const
    {
        return font_size_;
    }

    static text_position parse(const std::wstring & Str);

private:
    type type_;
    percent position_;
    
    bool has_font_size_;
    percent font_size_;
    
};

std::wostream & operator << (std::wostream & _Wostream, const text_position & _Val);

} 

APPLY_PARSE_XML_ATTRIBUTES(odf_types::text_position);

}

#endif
