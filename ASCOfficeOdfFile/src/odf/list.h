#ifndef _CPDOCCORE_ODF_LIST_H_
#define _CPDOCCORE_ODF_LIST_H_

#include <cpdoccore/CPOptional.h>
#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore {
namespace odf_reader {
namespace text {

// text:number
//////////////////////////////////////////////////////////////////////////////////////////////////

class number : public office_element_impl<number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextNumber;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    number() {}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ) {};
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name) {} ;
    virtual void add_text(const std::wstring & Text);

private:
    ::std::wstring string_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(number);


// text:list-item
//////////////////////////////////////////////////////////////////////////////////////////////////

class list_item;
typedef shared_ptr<list_item>::Type list_item_ptr;
typedef ::std::vector<list_item_ptr> list_item_ptr_array;

class list_item : public office_element_impl<list_item>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListItem;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
public:
    list_item() {} 

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text) {} ;

private:
    optional<unsigned int>::Type text_start_value_;

    office_element_ptr          text_number_;
    office_element_ptr_array    content_;

};

CP_REGISTER_OFFICE_ELEMENT2(list_item);

// text:list-header
//////////////////////////////////////////////////////////////////////////////////////////////////

class list_header;
typedef shared_ptr<list_header>::Type list_header_ptr;

class list_header : public office_element_impl<list_header>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListHeader;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    list_header() {} 

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text) {} ;

private:
    office_element_ptr          text_number_;
    office_element_ptr_array    content_;


};

CP_REGISTER_OFFICE_ELEMENT2(list_header);

}
}
}

#endif // #ifndef _CPDOCCORE_ODF_LIST_H_