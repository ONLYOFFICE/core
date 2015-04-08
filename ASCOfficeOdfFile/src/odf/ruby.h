#ifndef _CPDOCCORE_ODF_RUBY_H_
#define _CPDOCCORE_ODF_RUBY_H_

#include "office_elements.h"
#include "office_elements_create.h"

#include "datatypes/style_ref.h"

namespace cpdoccore {
namespace odf {
namespace text {

// text:ruby-base
//////////////////////////////////////////////////////////////////////////////////////////////////

class ruby_base : public office_element_impl<ruby_base>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextRubyBase;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
public:
    ruby_base() {}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    office_element_ptr_array paragraph_content_;
  
};

CP_REGISTER_OFFICE_ELEMENT2(ruby_base);

// text:ruby-text
//////////////////////////////////////////////////////////////////////////////////////////////////

class ruby_text : public office_element_impl<ruby_text>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextRubyText;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    ruby_text() {}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    style_ref text_style_name_;    
    office_element_ptr_array content_;
  
};

CP_REGISTER_OFFICE_ELEMENT2(ruby_text);

}
}
}

#endif // #ifndef _CPDOCCORE_ODF_RUBY_H_
