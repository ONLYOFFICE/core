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

#include "../../DataTypes/fontstyle.h"
#include "../../DataTypes/fontvariant.h"
#include "../../DataTypes/fontweight.h"
#include "../../DataTypes/fontstretch.h"
#include "../../DataTypes/fontfamilygeneric.h"
#include "../../DataTypes/fontpitch.h"

#include "../../DataTypes/common_attlists.h"


namespace cpdoccore { 
namespace odf_reader {

class common_svg_font_face_xlink_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    
	odf_types::common_xlink_attlist xlink_attlist_;
};
//  svg:font-face-uri
class svg_font_face_uri : public office_element_impl<svg_font_face_uri>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeSvgFontFaceUri;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

    common_svg_font_face_xlink_attlist	common_svg_font_face_xlink_attlist_;
    office_element_ptr_array			svg_font_face_format_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(svg_font_face_uri);

//  svg:desc
class svg_desc : public office_element_impl<svg_desc>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeSvgDesc;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

	std::wstring text_;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}
    virtual void add_text(const std::wstring & Text);
    virtual void add_space(const std::wstring & Text);
};
CP_REGISTER_OFFICE_ELEMENT2(svg_desc);

//	svg:font-face-format
class svg_font_face_format : public office_element_impl<svg_font_face_format>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeSvgFontFaceFormat;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

    optional<std::wstring>::Type svg_string_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(svg_font_face_format);

//  svg:font-face-name
class svg_font_face_name : public office_element_impl<svg_font_face_name>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeSvgFontFaceName;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    optional<std::wstring>::Type name_;
    
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(svg_font_face_name);

//  svg:definition-src
class svg_definition_src : public office_element_impl<svg_definition_src>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeSvgDefinitionSrc;

    CPDOCCORE_DEFINE_VISITABLE();

    common_svg_font_face_xlink_attlist common_svg_font_face_xlink_attlist_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(svg_definition_src);

//  style:font-face
class style_font_face : public office_element_impl<style_font_face>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleFontFace;

    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    
    optional<std::wstring>::Type			style_name_;
    optional<std::wstring>::Type			style_font_adornments_;
    optional<odf_types::font_family_generic>::Type style_font_family_generic_;
    optional<odf_types::font_pitch>::Type	style_font_pitch_;
    optional<std::wstring>::Type			style_font_charset_;
    
    optional<std::wstring>::Type			svg_font_family_;
    optional<odf_types::font_style>::Type	svg_font_style_;
    optional<odf_types::font_variant>::Type svg_font_variant_;
    optional<odf_types::font_weight>::Type	svg_font_weight_;
    optional<odf_types::font_stretch>::Type svg_font_stretch_;
    optional<odf_types::length>::Type		svg_font_size_;
    optional<std::wstring>::Type			svg_unicode_range_;
    optional<int>::Type						svg_units_per_em_;
    optional<std::wstring>::Type			svg_panose_1_;
    optional<int>::Type						svg_stemv_;
    optional<int>::Type						svg_stemh_;
    optional<int>::Type						svg_slope_;
    optional<int>::Type						svg_cap_height_;
    optional<int>::Type						svg_x_height_;
    optional<int>::Type						svg_accent_height_;
    optional<int>::Type						svg_ascent_;
    optional<int>::Type						svg_descent_;
    optional<std::wstring>::Type			svg_widths_;
    optional<std::wstring>::Type			svg_bbox_;
    optional<int>::Type						svg_ideographic_;
    optional<int>::Type						svg_alphabetic_;
    optional<int>::Type						svg_mathematical_;
    optional<int>::Type						svg_hanging_;
    optional<int>::Type						svg_v_ideographic_;
    optional<int>::Type						svg_v_alphabetic_;
    optional<int>::Type						svg_v_mathematical_;
    optional<int>::Type						svg_v_hanging_;
    optional<int>::Type						svg_underline_position_;
    optional<int>::Type						svg_underline_thickness_;
    optional<int>::Type						svg_strikethrough_position_;
    optional<int>::Type						svg_strikethrough_thickness_;
    optional<int>::Type						svg_overline_position_;
    optional<int>::Type						svg_overline_thickness_;

    office_element_ptr_array				svg_font_face_uri_; 
    office_element_ptr_array				svg_font_face_name_;   
    office_element_ptr						svg_definition_src_;

    friend class odf_document;
};
CP_REGISTER_OFFICE_ELEMENT2(style_font_face);

//  office-font-face-decls
class office_font_face_decls : public office_element_impl<office_font_face_decls>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeFontFaceDecls;

    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);

    office_element_ptr_array style_font_face_;

    friend class odf_document;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};
CP_REGISTER_OFFICE_ELEMENT2(office_font_face_decls);

}
}


