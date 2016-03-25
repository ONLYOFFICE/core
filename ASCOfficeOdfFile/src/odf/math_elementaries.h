#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf_reader {

class math_mstack : public office_element_impl<math_mstack>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMStack;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mstack);
CP_REGISTER_OFFICE_ELEMENT3(math_mstack);

//--------------------------------------------------------------------

class math_msrow : public office_element_impl<math_msrow>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMSRow;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_msrow);
CP_REGISTER_OFFICE_ELEMENT3(math_msrow);

//--------------------------------------------------------------------

class math_msline : public office_element_impl<math_msline>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMSLine;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_msline);
CP_REGISTER_OFFICE_ELEMENT3(math_msline);

//--------------------------------------------------------------------

class math_mlongdiv : public office_element_impl<math_mlongdiv>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMSLongDiv;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mlongdiv);
CP_REGISTER_OFFICE_ELEMENT3(math_mlongdiv);

//--------------------------------------------------------------------

class math_mscarries : public office_element_impl<math_mscarries>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMSCarries;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mscarries);
CP_REGISTER_OFFICE_ELEMENT3(math_mscarries);
//--------------------------------------------------------------------

class math_msgroup : public office_element_impl<math_msgroup>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMSGroup;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_msgroup);
CP_REGISTER_OFFICE_ELEMENT3(math_msgroup);

//--------------------------------------------------------------------

class math_mscarry : public office_element_impl<math_mscarry>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeMSCarry;

	CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context){}
	virtual void pptx_convert(oox::pptx_conversion_context & Context){}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    office_element_ptr_array    content_;
};

CP_REGISTER_OFFICE_ELEMENT2(math_mscarry);
CP_REGISTER_OFFICE_ELEMENT3(math_mscarry);

//--------------------------------------------------------------------
}
}
