
#include "list.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/utils.h>

namespace cpdoccore { 
namespace odf_reader {
namespace text {



// text:number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number::ns = L"text";
const wchar_t * number::name = L"number";

::std::wostream & number::text_to_stream(::std::wostream & _Wostream) const
{
    _Wostream << xml::utils::replace_xml_to_text( string_ );
    return _Wostream;
}

void number::add_text(const std::wstring & Text)
{
    string_ = Text;
}

// text:list-item
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * list_item::ns = L"text";
const wchar_t * list_item::name = L"list-item";

::std::wostream & list_item::text_to_stream(::std::wostream & _Wostream) const
{
    // TODO!!!!
    BOOST_FOREACH(const office_element_ptr & parElement, content_)
    {
        parElement->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void list_item::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:start-value", text_start_value_);
}

void list_item::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"number")
    {
        CP_CREATE_ELEMENT(text_number_);
    }
    else
    {
        CP_CREATE_ELEMENT(content_);        
    }
}

void list_item::docx_convert(oox::docx_conversion_context & Context)
{
    bool restart = false;
    // TODO - надо сделать так чтобы не только с 1
    if (text_start_value_)
    {
        restart = true;                    
    }

    Context.start_list_item(restart);

    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }

    Context.end_list_item();

}
void list_item::pptx_convert(oox::pptx_conversion_context & Context)
{
    bool restart = false;
    // TODO - надо сделать так чтобы не только с 1
    if (text_start_value_)
    {
        restart = true;                    
    }

    Context.get_text_context().start_list_item(restart);

    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->pptx_convert(Context);
    }

    Context.get_text_context().end_list_item();

}
// text:list-header
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * list_header::ns = L"text";
const wchar_t * list_header::name = L"list-header";

void list_header::docx_convert(oox::docx_conversion_context & Context)
{
	//заголовок это не элемент списка

	std::wstring s = Context.current_list_style();
	Context.end_list();

    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }

	Context.start_list(s, true);
}
void list_header::pptx_convert(oox::pptx_conversion_context & Context)
{
    bool restart = false;

	//заголовок это не элемент списка

    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->pptx_convert(Context);
    }

}


::std::wostream & list_header::text_to_stream(::std::wostream & _Wostream) const
{
    // TODO!!!!
    BOOST_FOREACH(const office_element_ptr & parElement, content_)
    {
        parElement->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void list_header::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void list_header::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"number")
    {
        CP_CREATE_ELEMENT(text_number_);
    }
    else
    {
        CP_CREATE_ELEMENT(content_);        
    }
}


}
}
}