/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#pragma once

#include <string>
#include "length.h"
#include "color.h"

namespace cpdoccore { namespace odf_types { 

class border_style
{
public:
    enum type
    {
        none,
        solid,
        dotted,
        dash,
        long_dash,
        dot_dash,
        dot_dot_dash,
        groove,
        double_,
        ridge,
        inset,
        outset,
        hidden,
        fine_dashed,
        double_thin
    };
    border_style();

    border_style(const std::wstring & Value);
    border_style(const border_style & Value);
    border_style(const color & color_,  const type & style_, const length & length_);

    static border_style parse(const std::wstring & Str);
    border_style& operator=(const border_style& ob);
public:
    bool initialized() const;
    bool is_none()     const;

    const length     & get_length()const;
    const type       & get_style() const;
    const color      & get_color() const;

private:
    bool        none_ = true;
    bool        initialized_ = false;
    length      length_;
    type        style_ = none;
    color       color_;
};

std::wostream & operator << (std::wostream & _Wostream, const border_style & _Val);
bool            operator == (border_style & _b1, border_style & _b2);

} 
    APPLY_PARSE_XML_ATTRIBUTES(odf_types::border_style);

}
