#pragma once

#include "math_elementaries.h"
#include "math_token_elements.h"
#include "math_table_elements.h"
#include "math_limit_elements.h"
#include "math_layout_elements.h"

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf_reader {

class office_math : public office_element_impl<office_math>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMath;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

	office_element_ptr semantics_;
};

CP_REGISTER_OFFICE_ELEMENT2(office_math);
CP_REGISTER_OFFICE_ELEMENT3(office_math);
//--------------------------------------------------------------------

class math_semantics : public office_element_impl<math_semantics>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMathSemantics;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
	office_element_ptr			annotation_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_semantics);
CP_REGISTER_OFFICE_ELEMENT3(math_semantics);
//-------------------------------------------------------------------------------------------

class math_annotation : public office_element_impl<math_annotation>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMathAnnotation;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

    office_element_ptr_array    content_;	
	_CP_OPT(std::wstring)		text_;

	_CP_OPT(std::wstring)		encoding_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_annotation);
CP_REGISTER_OFFICE_ELEMENT3(math_annotation);
//--------------------------------------------------------------------
class math_annotation_xml : public office_element_impl<math_annotation_xml>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMathAnnotationXml;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

    office_element_ptr_array    content_;
	_CP_OPT(std::wstring)		text_;

	_CP_OPT(std::wstring)		encoding_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_annotation_xml);
CP_REGISTER_OFFICE_ELEMENT3(math_annotation_xml);
//--------------------------------------------------------------------
}
}
