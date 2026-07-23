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
