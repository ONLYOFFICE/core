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
#include <CPOptional.h>

namespace cpdoccore { 

namespace odf_reader {

	class text_format_properties_content;
	typedef boost::shared_ptr<text_format_properties_content> text_format_properties_content_ptr;

	class paragraph_format_properties;
	class style_table_cell_properties_attlist;
}

namespace oox {

	class xlsx_conversion_context;

struct xlsx_alignment
{
    _CP_OPT(std::wstring)	horizontal;
    _CP_OPT(int)			indent;
    _CP_OPT(bool)			justifyLastLine;
    _CP_OPT(int)			readingOrder;
    _CP_OPT(int)			relativeIndent;
    _CP_OPT(bool)			shrinkToFit;
    _CP_OPT(unsigned int)	textRotation;
    _CP_OPT(std::wstring)	vertical;
    _CP_OPT(bool)			wrapText;   

    bool operator == (const xlsx_alignment & rVal) const;
    bool operator != (const xlsx_alignment & rVal) const;

    friend void xlsx_serialize(std::wostream & _Wostream, const xlsx_alignment & alignment);
    friend bool is_default(const xlsx_alignment & rVal);
    friend std::size_t hash_value(xlsx_alignment const & val);
};
xlsx_alignment OdfProperties2XlsxAlignment( xlsx_conversion_context									* context,
											const odf_reader::text_format_properties_content_ptr	textProp, 
											const odf_reader::paragraph_format_properties			* parProp,
											const odf_reader::style_table_cell_properties_attlist	* cellProp);

}
}
