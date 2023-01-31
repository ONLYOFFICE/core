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
		xlsx_fill(	const odf_reader::graphic_format_properties				* graphProp,
					const odf_reader::style_table_cell_properties_attlist	* cellProp);
       
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
