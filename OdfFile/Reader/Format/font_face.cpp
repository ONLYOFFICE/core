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

#include "font_face.h"

#include <xml/xmlchar.h>
#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf_reader {

const wchar_t * svg_desc::ns	= L"svg";
const wchar_t * svg_desc::name	= L"desc";

std::wostream & svg_desc::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    _Wostream <<  text_ ;
    return _Wostream;
}

void svg_desc::add_text(const std::wstring & Text) 
{
    text_ += Text;
}
void svg_desc::add_space(const std::wstring & Text) 
{
    text_ += Text;
}
// svg:font-face-uri
//---------------------------------------------------------------------------------------
const wchar_t * svg_font_face_uri::ns = L"svg";
const wchar_t * svg_font_face_uri::name = L"font-face-uri";

std::wostream & svg_font_face_uri::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return _Wostream;
}

void svg_font_face_uri::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_svg_font_face_xlink_attlist_.add_attributes(Attributes);
}

void svg_font_face_uri::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"svg" == Ns && L"font-face-format" == Name)
    {
        CP_CREATE_ELEMENT(svg_font_face_format_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}

// svg:font-face-format
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * svg_font_face_format::ns = L"svg";
const wchar_t * svg_font_face_format::name = L"font-face-format";

std::wostream & svg_font_face_format::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return _Wostream;
}

void svg_font_face_format::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"svg:string", svg_string_);
}

void svg_font_face_format::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

// svg:font-face-name
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * svg_font_face_name::ns = L"svg";
const wchar_t * svg_font_face_name::name = L"font-face-name";

std::wostream & svg_font_face_name::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return _Wostream;
}

void svg_font_face_name::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"name", name_);
}

void svg_font_face_name::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

    
//////////////////////////////////////////////////////////////////////////////////////////////////

void common_svg_font_face_xlink_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    xlink_attlist_.add_attributes(Attributes);
}

// svg:definition-src
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * svg_definition_src::ns = L"svg";
const wchar_t * svg_definition_src::name = L"definition-src";

void svg_definition_src::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    common_svg_font_face_xlink_attlist_.add_attributes(Attributes);
}

void svg_definition_src::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

// style:font-face
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_font_face::ns = L"style";
const wchar_t * style_font_face::name = L"font-face";

void style_font_face::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:name", style_name_);
    CP_APPLY_ATTR(L"style:font-adornments", style_font_adornments_);
    CP_APPLY_ATTR(L"style:font-family-generic", style_font_family_generic_);
    CP_APPLY_ATTR(L"style:font-pitch", style_font_pitch_);
    CP_APPLY_ATTR(L"style:font-charset", style_font_charset_);

    CP_APPLY_ATTR(L"svg:font-family",             svg_font_family_);
    CP_APPLY_ATTR(L"svg:font-style",              svg_font_style_);
    CP_APPLY_ATTR(L"svg:font-variant",            svg_font_variant_);
    CP_APPLY_ATTR(L"svg:font-weight",             svg_font_weight_);
    CP_APPLY_ATTR(L"svg:font-stretch",            svg_font_stretch_);
    CP_APPLY_ATTR(L"svg:font-size",               svg_font_size_);
    CP_APPLY_ATTR(L"svg:unicode-range",           svg_unicode_range_);
    CP_APPLY_ATTR(L"svg:units-per-em",            svg_units_per_em_);
    CP_APPLY_ATTR(L"svg:panose-1",                svg_panose_1_);
    CP_APPLY_ATTR(L"svg:stemv",                   svg_stemv_);
    CP_APPLY_ATTR(L"svg:stemh",                   svg_stemh_);
    CP_APPLY_ATTR(L"svg:slope",                   svg_slope_);
    CP_APPLY_ATTR(L"svg:cap-height",              svg_cap_height_);
    CP_APPLY_ATTR(L"svg:x-height",                svg_x_height_);
    CP_APPLY_ATTR(L"svg:accent-height",           svg_accent_height_);
    CP_APPLY_ATTR(L"svg:ascent",                  svg_ascent_);
    CP_APPLY_ATTR(L"svg:descent",                 svg_descent_);
    CP_APPLY_ATTR(L"svg:widths",                  svg_widths_);
    CP_APPLY_ATTR(L"svg:bbox",                    svg_bbox_);
    CP_APPLY_ATTR(L"svg:ideographic",             svg_ideographic_);
    CP_APPLY_ATTR(L"svg:alphabetic",              svg_alphabetic_);
    CP_APPLY_ATTR(L"svg:mathematical",            svg_mathematical_);
    CP_APPLY_ATTR(L"svg:hanging",                 svg_hanging_);
    CP_APPLY_ATTR(L"svg:v-ideographic",           svg_v_ideographic_);
    CP_APPLY_ATTR(L"svg:v-alphabetic",            svg_v_alphabetic_);
    CP_APPLY_ATTR(L"svg:v-mathematical",          svg_v_mathematical_);
    CP_APPLY_ATTR(L"svg:v-hanging",               svg_v_hanging_);
    CP_APPLY_ATTR(L"svg:underline-position",      svg_underline_position_);
    CP_APPLY_ATTR(L"svg:underline-thickness",     svg_underline_thickness_);
    CP_APPLY_ATTR(L"svg:strikethrough-position",  svg_strikethrough_position_);
    CP_APPLY_ATTR(L"svg:strikethrough-thickness", svg_strikethrough_thickness_);
    CP_APPLY_ATTR(L"svg:overline-position",       svg_overline_position_);
    CP_APPLY_ATTR(L"svg:overline-thickness",      svg_overline_thickness_);

}

void style_font_face::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"svg" == Ns && L"font-face-uri" == Name)
        CP_CREATE_ELEMENT(svg_font_face_uri_);
    else if (L"svg" == Ns && L"font-face-name" == Name)
         CP_CREATE_ELEMENT(svg_font_face_name_);    
    else if (L"svg" == Ns && L"definition-src" == Name)
         CP_CREATE_ELEMENT(svg_definition_src_);            
    else
        CP_NOT_APPLICABLE_ELM();
}

void style_font_face::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();

    if (style_name_ && !style_name_->empty() && svg_font_family_ && !svg_font_family_->empty())
    {

        std::wstring w_name;

        strm << L"<w:font>";

        strm << L"</w:font>";
    }        
}

// office:font-face-decls
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_font_face_decls::ns = L"office";
const wchar_t * office_font_face_decls::name = L"font-face-decls";

void office_font_face_decls::add_attributes( const xml::attributes_wc_ptr & Attributes )
{}

void office_font_face_decls::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if (L"style" == Ns && L"font-face" == Name)
    {
        CP_CREATE_ELEMENT(style_font_face_);            
    } 
    else
        CP_NOT_APPLICABLE_ELM();
}

void office_font_face_decls::docx_convert(oox::docx_conversion_context & Context)
{
    std::wostream & strm = Context.output_stream();
    
    strm << L"<w:fonts \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
mc:Ignorable=\"w14\">";

 	for (size_t i = 0; i < style_font_face_.size(); i++)
    {
        style_font_face_[i]->docx_convert(Context);        
    }

    strm << L"</w:fonts>";

}

}
}
