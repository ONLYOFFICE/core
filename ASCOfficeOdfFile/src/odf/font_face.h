#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"

#include "datatypes/fontstyle.h"
#include "datatypes/fontvariant.h"
#include "datatypes/fontweight.h"
#include "datatypes/fontstretch.h"
#include "datatypes/length.h"
#include "datatypes/fontfamilygeneric.h"
#include "datatypes/fontpitch.h"

#include "datatypes/common_attlists.h"


namespace cpdoccore { 
namespace odf_reader {

class common_svg_font_face_xlink_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    
private:
	odf_types::common_xlink_attlist common_xlink_attlist_;
};

/// \class  svg_font_face_uri
/// \brief  svg:font-face-uri
class svg_font_face_uri : public office_element_impl<svg_font_face_uri>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeSvgFontFaceUri;

    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    common_svg_font_face_xlink_attlist common_svg_font_face_xlink_attlist_;
    office_element_ptr_array svg_font_face_format_;

};

CP_REGISTER_OFFICE_ELEMENT2(svg_font_face_uri);

/// \class  svg_font_face_format
/// \brief  svg:font-face-format
class svg_font_face_format : public office_element_impl<svg_font_face_format>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeSvgFontFaceFormat;

    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    optional<std::wstring>::Type svg_string_;

};

CP_REGISTER_OFFICE_ELEMENT2(svg_font_face_format);

/// \class  svg_font_face_name
/// \brief  svg:font-face-name
class svg_font_face_name : public office_element_impl<svg_font_face_name>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeSvgFontFaceName;

    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
    
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    optional<std::wstring>::Type name_;

};

CP_REGISTER_OFFICE_ELEMENT2(svg_font_face_name);

/// \class  svg_definition_src
/// \brief  svg:definition-src
class svg_definition_src : public office_element_impl<svg_definition_src>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeSvgDefinitionSrc;

    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    common_svg_font_face_xlink_attlist common_svg_font_face_xlink_attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(svg_definition_src);


/// \class  style_font_face
/// \brief  style:font-face
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
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    
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

private:
    // svg-font-face-src
    
    office_element_ptr_array svg_font_face_uri_; // svg-font-face-uri
    office_element_ptr_array svg_font_face_name_; // svg-font-face-name    

    //svg-definition-src
    office_element_ptr svg_definition_src_;

    friend class odf_document;
};

CP_REGISTER_OFFICE_ELEMENT2(style_font_face);

/// \class  office_font_face_decls
/// \brief  office-font-face-decls
class office_font_face_decls : public office_element_impl<office_font_face_decls>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeFontFaceDecls;

    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    office_element_ptr_array style_font_face_;

    friend class odf_document;

};

CP_REGISTER_OFFICE_ELEMENT2(office_font_face_decls);

}
}


