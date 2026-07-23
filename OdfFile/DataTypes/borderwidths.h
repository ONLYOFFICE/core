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
