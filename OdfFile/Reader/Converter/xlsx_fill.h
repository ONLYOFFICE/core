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

#include "xlsx_color.h"

namespace cpdoccore { 
namespace odf_reader {

    class paragraph_format_properties;
    class style_table_cell_properties_attlist;
	class graphic_format_properties;
}
}

namespace cpdoccore {
namespace oox {

    struct xlsx_patternFill
    {
        _CP_OPT(xlsx_color)		fgColor;
        _CP_OPT(xlsx_color)		bgColor;
        _CP_OPT(std::wstring)	patternType;

        bool operator == (const xlsx_patternFill & rVal) const;
        bool operator != (const xlsx_patternFill & rVal) const;
        friend std::size_t hash_value(xlsx_patternFill const & val);
    };

    struct xlsx_gradientFill
    {
        bool operator == (const xlsx_gradientFill & rVal) const;
        bool operator != (const xlsx_gradientFill & rVal) const;
        friend std::size_t hash_value(xlsx_gradientFill const & val);
    };

    struct xlsx_fill
    {
		xlsx_fill() {}
		xlsx_fill(	const odf_reader::graphic_format_properties *graphProp,
					const odf_reader::style_table_cell_properties_attlist	*cellProp);
       
		_CP_OPT(xlsx_patternFill)	patternFill;
        _CP_OPT(xlsx_gradientFill)	gradientFill;

        std::size_t index;
        bool		bDefault;

		bool bEnabled;

        bool operator == (const xlsx_fill & rVal) const;
        bool operator != (const xlsx_fill & rVal) const;
        
		friend std::size_t hash_value(xlsx_fill const & val);
    };

    void xlsx_serialize(std::wostream & _Wostream, const xlsx_gradientFill  & gradientFill);
    void xlsx_serialize(std::wostream & _Wostream, const xlsx_patternFill   & patternFill);
    void xlsx_serialize(std::wostream & _Wostream, const xlsx_fill          & fill);

}
}
