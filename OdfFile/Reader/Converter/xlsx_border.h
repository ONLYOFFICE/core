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
#include <vector>

#include "xlsx_color.h"

namespace cpdoccore { 
namespace odf_reader {

class text_format_properties;
class paragraph_format_properties;
class style_table_cell_properties;

}
}

namespace cpdoccore {
namespace oox {

    struct xlsx_border_edge
    {
        xlsx_border_edge()
        {}

        xlsx_border_edge(std::wstring _style, xlsx_color _color = xlsx_color()) : style(_style), color(_color)
        {}

        _CP_OPT(std::wstring)	style; // attribute
        _CP_OPT(xlsx_color)		color; // element
        _CP_OPT(int)			width; //

        bool operator == (const xlsx_border_edge & rVal) const;
        bool operator != (const xlsx_border_edge & rVal) const;
    };

    struct xlsx_border
    {
        _CP_OPT(bool) diagonalUp;
        _CP_OPT(bool) diagonalDown;
        _CP_OPT(bool) outline; // default true

        _CP_OPT(xlsx_border_edge) left;
        _CP_OPT(xlsx_border_edge) right;
        _CP_OPT(xlsx_border_edge) top;
        _CP_OPT(xlsx_border_edge) bottom;
        _CP_OPT(xlsx_border_edge) diagonal;
        _CP_OPT(xlsx_border_edge) vertical;
        _CP_OPT(xlsx_border_edge) horizontal;

        std::size_t index;

        bool operator == (const xlsx_border & rVal) const;
        bool operator != (const xlsx_border & rVal) const;

    };

    std::size_t hash_value(const _CP_OPT(xlsx_border_edge) & val);
    std::size_t hash_value(xlsx_border_edge const& val);
    std::size_t hash_value(xlsx_border const& val);

    bool is_default(xlsx_border_edge * borderEdge);
    bool is_default(xlsx_border_edge const& borderEdge);
    bool is_default(const _CP_OPT(xlsx_border_edge) & borderEdge);

     bool is_default(xlsx_border const& border);

    void xlsx_serialize(std::wostream & _Wostream, xlsx_border const & border);

}
}
