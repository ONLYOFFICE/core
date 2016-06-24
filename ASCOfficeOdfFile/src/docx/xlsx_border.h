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
#include <cpdoccore/CPOptional.h>
#include <string>
#include <vector>

#include "xlsx_color.h"

namespace cpdoccore { 
namespace odf_reader {

class text_format_properties_content;
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
