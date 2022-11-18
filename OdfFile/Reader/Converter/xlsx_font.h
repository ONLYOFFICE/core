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

#include <iosfwd>
#include <string>
#include <vector>

#include "xlsx_color.h"

namespace cpdoccore { 
namespace odf_reader {

	class text_format_properties_content;
	typedef boost::shared_ptr<text_format_properties_content> text_format_properties_content_ptr;

	class paragraph_format_properties;
    class style_table_cell_properties_attlist;
}
}

namespace cpdoccore {
namespace oox {

    enum xlsx_vertAlign
    {
        vertAlignBaseline,
        vertAlignSubscript,
        vertAlignSuperscript
    };

    enum XlsxFontFamily
    {
        XFAMILY_EMPTY = -1,
        XFAMILY_NOTAPPLICABLE = 0,
        XFAMILY_ROMAN = 1,
        XFAMILY_SWISS = 2,
        XFAMILY_MODERN = 3,
        XFAMILY_SCRIPT = 4,
        XFAMILY_DECORATIVE = 5
    };

    enum XlsxUnderline
    {
        XUNDERLINE_NONE,
        XUNDERLINE_SINGLE,
        XUNDERLINE_DOUBLE,
        XUNDERLINE_SINGLE_ACCOUNTING,
        XUNDERLINE_DOUBLE_ACCOUNTING
    };

    enum XlsxFontCharset
    {
        XCHARSET_EMPTY = -1,
        XCHARSET_CHARSET_ANSI = 0,
        XCHARSET_DEFAULT = 1,
        XCHARSET_SYMBOL = 2,
        XCHARSET_MAC = 77,
        XCHARSET_SHIFTJIS = 128,
        XCHARSET_HANGEUL = 129,
        XCHARSET_JOHAB = 130,
        XCHARSET_GB2312 = 134,
        XCHARSET_CHINESEBIG5 = 136,
        XCHARSET_GREEK = 161,
        XCHARSET_TURKISH = 162,
        XCHARSET_VIETNAMESE = 163,
        XCHARSET_HEBREW = 177,
        XCHARSET_ARABIC = 178,
        XCHARSET_BALTIC = 186,
        XCHARSET_RUSSIAN = 204,
        XCHARSET_THAI = 222,
        XCHARSET_EASTEUROPE = 238,
        XCHARSET_OEM = 255
    };

    struct xlsx_font
    {
		xlsx_font () {};
		xlsx_font (	const odf_reader::text_format_properties_content_ptr	textProp,
					const odf_reader::paragraph_format_properties			* parProp,
					const odf_reader::style_table_cell_properties_attlist	* cellProp);

		bool					bEnabled;

        _CP_OPT(bool)			bold;
        _CP_OPT(unsigned int)	charset;
        _CP_OPT(xlsx_color)		color;
        _CP_OPT(bool)			condense;
        _CP_OPT(bool)			extend;
        _CP_OPT(XlsxFontFamily) family;
        _CP_OPT(bool)			i; ///< italic
        _CP_OPT(std::wstring)	name;
        _CP_OPT(bool)			outline;
        _CP_OPT(std::wstring)	scheme;
        _CP_OPT(bool)			shadow;
        _CP_OPT(bool)			strike;
        _CP_OPT(double)			sz;
        _CP_OPT(XlsxUnderline)	u;
        _CP_OPT(xlsx_vertAlign) vertAlign;

        mutable std::size_t index;

        bool operator == (const xlsx_font & rVal) const;
        bool operator != (const xlsx_font & rVal) const;

        friend std::size_t hash_value(xlsx_font const & val);
    };

    void xlsx_serialize(std::wostream & _Wostream, const xlsx_font & font);
}
}

