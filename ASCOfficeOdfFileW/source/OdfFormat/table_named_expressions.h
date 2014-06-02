#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"
#include "style_ref.h"

namespace cpdoccore { 
namespace odf {

/// \brief  table:named-expressions
class table_named_expressions : public office_element_impl<table_named_expressions>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableNamedExpressions;

    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array named_range_;
    office_element_ptr_array named_expression_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_named_expressions);

/// \brief  table:named-range
class table_named_range : public office_element_impl<table_named_range>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableNamedRange;

    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    _CP_OPT(std::wstring) table_name_;
    _CP_OPT(std::wstring) table_cell_range_address_;
    _CP_OPT(std::wstring) table_base_cell_address_;

	_CP_OPT(std::wstring) table_range_usable_as_;//printrange, filter , repeatrow, repeatcolumn через пробел

};

CP_REGISTER_OFFICE_ELEMENT2(table_named_range);


/// \brief  table:named-expression
class table_named_expression : public office_element_impl<table_named_expression>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableNamedExpression;

    CPDOCCORE_DEFINE_VISITABLE();

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name){}
	virtual void add_child_element( office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

    _CP_OPT(std::wstring) table_name_;
    _CP_OPT(std::wstring) table_expression_;
    _CP_OPT(std::wstring) table_base_cell_address_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_named_expression);



}
}
