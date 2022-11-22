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
    bool GetResolution(const wchar_t* fileName, int & Width, int &Height, NSFonts::IApplicationFonts *appFonts);
	void GenerateZeroImage(const std::wstring & fileName);
}

namespace cpdoccore { 
namespace odf_reader {

class styles_lite_container;


enum BorderSide { sideTop, sideBottom, sideLeft, sideRight, sideMiddle };

int get_value_emu(const _CP_OPT(odf_types::length) & len);
int get_value_emu(double pt);

int Compute_BorderWidth(const graphic_format_properties & graphicProperties, BorderSide borderSide);//emu

int GetMargin(const graphic_format_properties & graphicProperties, BorderSide borderSide);//emu

void Compute_GraphicFill(const odf_types::common_draw_fill_attlist & props, 
						 const office_element_ptr & style_image, styles_lite_container &styles, oox::_oox_fill & fill, bool txbx = false, bool reset_fill = true);

typedef double double_4[4];
bool parse_clipping(std::wstring strClipping, std::wstring fileName, double_4 & clip_rect, NSFonts::IApplicationFonts *appFonts);
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

    std::wstring							office_name_;
    _CP_OPT(odf_types::target_frame_name)	office_target_frame_name_;

    std::wstring					text_style_name_;
    std::wstring					text_visited_style_name_;

};
CP_REGISTER_OFFICE_ELEMENT2(draw_a);

void docx_convert_transforms(std::wstring transformStr, std::vector<odf_reader::_property> & additional);
void pptx_convert_transforms(std::wstring transformStr, oox::pptx_conversion_context & Context);
void xlsx_convert_transforms(std::wstring transformStr, oox::xlsx_conversion_context & Context);

}
}
