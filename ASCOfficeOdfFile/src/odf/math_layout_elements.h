#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

#include "datatypes/common_attlists.h"
#include "datatypes/fontstyle.h"
#include "datatypes/fontweight.h"

namespace cpdoccore { 
	
	using namespace odf_types;

namespace odf_reader {

class math_mrow : public office_element_impl<math_mrow>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMRow;

	math_mrow();
	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
	bool						next_element_to_prev_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mrow);
CP_REGISTER_OFFICE_ELEMENT3(math_mrow);
//--------------------------------------------------------------------------------------
class math_mfrac : public office_element_impl<math_mfrac>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMFrac;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;	//2 elements 

	_CP_OPT(length)				linethickness;
	//numalign left, central,right
	//denomalign
	_CP_OPT(Bool)				bevelled;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mfrac);
CP_REGISTER_OFFICE_ELEMENT3(math_mfrac);
//--------------------------------------------------------------------------------------
class math_msqrt : public office_element_impl<math_msqrt>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMSqrt;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
};
CP_REGISTER_OFFICE_ELEMENT2(math_msqrt);
CP_REGISTER_OFFICE_ELEMENT3(math_msqrt);

//--------------------------------------------------------------------------------------
class math_mroot : public office_element_impl<math_mroot>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMRoot;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
};
CP_REGISTER_OFFICE_ELEMENT2(math_mroot);
CP_REGISTER_OFFICE_ELEMENT3(math_mroot);

//--------------------------------------------------------------------------------------
class math_mstyle : public office_element_impl<math_mstyle>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMStyle;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;

	_CP_OPT(font_weight)		fontweight_;
};
CP_REGISTER_OFFICE_ELEMENT2(math_mstyle);
CP_REGISTER_OFFICE_ELEMENT3(math_mstyle);
//--------------------------------------------------------------------------------------

class math_menclose : public office_element_impl<math_menclose>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMEnClose;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
};
CP_REGISTER_OFFICE_ELEMENT2(math_menclose);
CP_REGISTER_OFFICE_ELEMENT3(math_menclose);
//--------------------------------------------------------------------------------------

class math_mfenced : public office_element_impl<math_mfenced>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMFenced;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
};
CP_REGISTER_OFFICE_ELEMENT2(math_mfenced);
CP_REGISTER_OFFICE_ELEMENT3(math_mfenced);
//--------------------------------------------------------------------------------------

class math_mpadded : public office_element_impl<math_mpadded>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMPadded;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
};
CP_REGISTER_OFFICE_ELEMENT2(math_mpadded);
CP_REGISTER_OFFICE_ELEMENT3(math_mpadded);
//--------------------------------------------------------------------------------------
//merror
//mphantom
}
}
