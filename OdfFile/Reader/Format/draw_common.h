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

#include "office_elements.h"
#include "office_elements_create.h"

#include "../../Reader/Converter/xlsxconversioncontext.h"
#include "../../Reader/Converter/xlsx_utils.h"

#include "style_graphic_properties.h"
#include "draw_frame.h"
#include "draw_shapes.h"

#include "../../DataTypes/xlink.h"
#include "../../DataTypes/targetframename.h"
#include "../../DataTypes/common_attlists.h"

namespace _image_file_
{
    bool GetResolution(const wchar_t* fileName, _CP_OPT(int) &Width, _CP_OPT(int) &Height, NSFonts::IApplicationFonts *appFonts);
	void GenerateZeroImage(const std::wstring & fileName);
}

namespace cpdoccore { 
namespace odf_reader {

class styles_lite_container;


enum BorderSide { sideTop, sideBottom, sideLeft, sideRight, sideMiddle };

int get_value_emu(const _CP_OPT(odf_types::length) & len);
int get_value_emu(double pt);

int Compute_BorderWidth(const graphic_format_properties_ptr & graphicProperties, BorderSide borderSide);//emu

int GetMargin(const graphic_format_properties_ptr & graphicProperties, BorderSide borderSide);//emu

void Compute_GraphicFill(const odf_types::common_draw_fill_attlist & props, 
						 const office_element_ptr & style_image, odf_document *document, oox::_oox_fill & fill, bool txbx = false, bool reset_fill = true);

typedef double double_4[4];
bool parse_clipping(std::wstring strClipping, int width, int height, double_4 & clip_rect);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_a : public office_element_impl<draw_a>
{  
public:
    static const wchar_t * ns;
    static const wchar_t * name;
   
	static const ElementType type = typeDrawA;
	static const xml::NodeType xml_type = xml::typeElement;
	CPDOCCORE_DEFINE_VISITABLE();

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

  
	office_element_ptr_array content_;

	friend class odf_document;

private:
	odf_types::common_xlink_attlist xlink_attlist_;

    std::wstring office_name_;
    _CP_OPT(odf_types::target_frame_name) office_target_frame_name_;

    std::wstring text_style_name_;
    std::wstring text_visited_style_name_;

};
CP_REGISTER_OFFICE_ELEMENT2(draw_a);

void docx_convert_transforms(std::wstring transformStr, std::vector<odf_reader::_property> & additional);
void pptx_convert_transforms(std::wstring transformStr, oox::pptx_conversion_context & Context);
void xlsx_convert_transforms(std::wstring transformStr, oox::xlsx_conversion_context & Context);

}
}
