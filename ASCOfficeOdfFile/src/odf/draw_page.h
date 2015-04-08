#pragma once

#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "datatypes/presentationclass.h"


namespace cpdoccore { 
namespace odf {


class draw_page_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
	_CP_OPT(std::wstring)		draw_name_;
	_CP_OPT(std::wstring)		draw_id_;
	_CP_OPT(std::wstring)		draw_style_name_;

	_CP_OPT(std::wstring)		page_layout_name_;
	_CP_OPT(std::wstring)		master_page_name_;

	_CP_OPT(std::wstring)		use_footer_name_;
	_CP_OPT(std::wstring)		use_date_time_name_;
};
/// \class  draw_page
class draw_page : public office_element_impl<draw_page>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawPage;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
	void pptx_convert_placeHolder(oox::pptx_conversion_context & Context, std::wstring styleName, presentation_class::type PresentationClass);

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    office_element_ptr_array content_;
	office_element_ptr animation_;

	draw_page_attr draw_page_attr_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_page);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//presentation:footer-decl
class presentation_footer_decl : public office_element_impl<presentation_footer_decl>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typePresentationFooterDecl;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void pptx_convert(oox::pptx_conversion_context & Context);

	_CP_OPT(std::wstring)	presentation_name_;
	std::wstring			text_;

private:
	virtual void add_text(const std::wstring & Text);
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name){}


};
CP_REGISTER_OFFICE_ELEMENT2(presentation_footer_decl);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//presentation:date-time-decl
class presentation_date_time_decl : public office_element_impl<presentation_date_time_decl>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typePresentationDateTimeDecl;
    CPDOCCORE_DEFINE_VISITABLE();

	_CP_OPT(std::wstring)	presentation_name_;
	_CP_OPT(std::wstring)	presentation_source_;
	_CP_OPT(std::wstring)	style_data_style_name_;

	std::wstring			text_;
    
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
	virtual void add_text(const std::wstring & Text);
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name){}


};
CP_REGISTER_OFFICE_ELEMENT2(presentation_date_time_decl);

}
}
