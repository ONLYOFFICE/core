#pragma once

#include <cpdoccore/CPOptional.h>
#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore {
namespace odf {

// text:number
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_number : public office_element_impl<text_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextNumber;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    text_number() {}

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    virtual void add_text(const std::wstring & Text);

    ::std::wstring string_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(text_number);


// text:list-item
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_list_item;
typedef shared_ptr<text_list_item>::Type text_list_item_ptr;
typedef ::std::vector<text_list_item_ptr> text_list_item_ptr_array;

class text_list_item : public office_element_impl<text_list_item>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListItem;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

public:
    text_list_item() {} 

    virtual void add_text(const std::wstring & Text) {} ;

    _CP_OPT(unsigned int)		text_start_value_;

    office_element_ptr          text_number_;
    office_element_ptr_array    content_;

};

CP_REGISTER_OFFICE_ELEMENT2(text_list_item);

// text:list-header
//////////////////////////////////////////////////////////////////////////////////////////////////

class text_list_header;
typedef shared_ptr<text_list_header>::Type text_list_header_ptr;

class text_list_header : public office_element_impl<text_list_header>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListHeader;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

public:
    text_list_header() {} 

    virtual void add_text(const std::wstring & Text) {} ;

private:
    office_element_ptr          text_number_;
    office_element_ptr_array    content_;


};

CP_REGISTER_OFFICE_ELEMENT2(text_list_header);


}
}

