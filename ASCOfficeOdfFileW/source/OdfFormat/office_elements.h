#pragma once

#include <vector>
#include <boost/algorithm/string/replace.hpp>

#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPWeakPtr.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/attributes.h>

#include "office_elements_type.h"

#include "visitor.h"

namespace cpdoccore {
namespace odf_writer {

class odf_conversion_context;

class office_element;

typedef shared_ptr<office_element>::Type office_element_ptr;
typedef weak_ptr<office_element>::Type office_element_weak_ptr;
typedef std::vector<office_element_ptr> office_element_ptr_array;

class office_element : public xml::element<wchar_t>, public base_visitable, boost::noncopyable 
{    
public:
    office_element() : context_(NULL) {}

    virtual ElementType get_type() const = 0;
    virtual ~office_element() = 0;

	virtual void serialize(std::wostream & _Wostream) = 0 ;

	void setContext(odf_conversion_context * Context) { context_ = Context; }
    odf_conversion_context * getContext() { return context_; }
    const odf_conversion_context * getContext() const { return context_; }

	virtual void add_child_element( const office_element_ptr & child) = 0;
	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name)=0;

    virtual void add_text(const std::wstring & Text) {}

	void set_root(bool root){is_root_ = root;}
////////////////////////
    virtual ::std::wostream & serialize(::std::wostream & _Wostream) const
    {
        _CP_LOG << L"[warning] use base text_to_stream\n";
        return _Wostream;
    }

    virtual ::std::wostream & xml_to_stream(::std::wostream & _Wostream) const
    {
        _CP_LOG << L"[warning] use base xml_to_stream\n";
        return _Wostream;
    }
private:
    bool is_root_;	
	odf_conversion_context * context_;
};



#define CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_ public:\
    virtual const wchar_t * get_ns() const { return ns; }\
    virtual const wchar_t * get_name() const { return name; }\
    virtual xml::NodeType get_xml_type() const { return xml_type; }\
    virtual ElementType get_type() const { return type; }



inline office_element::~office_element()
{
}

/// \class  office_element_impl
template <class Element>
class office_element_impl : public virtual office_element
{
// xml::element impl
public:
    virtual const wchar_t * get_ns() const
    { 
        return Element::ns;
    }
    
    virtual const wchar_t * get_name() const
    { 
        return Element::name;
    }

    virtual xml::NodeType get_xml_type() const
    { 
        return Element::xml_type;
    }
    virtual void add_child_element( const office_element_ptr & child)
    {
        _CP_LOG << L"Non add child in " << Element::ns << L":" << Element::name << std::endl;
    }
private:

// office_element impl
public:
    virtual ElementType get_type() const
    { 
        return Element::type;
    }
   
};


}
}

