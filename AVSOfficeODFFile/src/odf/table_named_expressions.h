#ifndef _CPDOCCORE_TABLE_NAMED_EXPRESSIONS_H_
#define _CPDOCCORE_TABLE_NAMED_EXPRESSIONS_H_

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"
#include "style_ref.h"

namespace cpdoccore { 
namespace odf {

/// \class  table_named_expressions
/// \brief  table:named-expressions
///         table-named-expressions
class table_named_expressions : public office_element_impl<table_named_expressions>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableNamedExpressions;

    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    office_element_ptr_array named_range_;
    office_element_ptr_array named_expression_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_named_expressions);

/// \class  table_named_range
/// \brief  table:named-range
///         table-named-range
class table_named_range : public office_element_impl<table_named_range>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableNamedRange;

    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    _CP_OPT(std::wstring) table_name_;
    _CP_OPT(std::wstring) table_cell_range_address_;
    _CP_OPT(std::wstring) table_base_cell_address_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_named_range);


/// \class  table_named_expression
/// \brief  table:named-expression
///         table-named-expression
class table_named_expression : public office_element_impl<table_named_expression>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableNamedExpression;

    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    _CP_OPT(std::wstring) table_name_;
    _CP_OPT(std::wstring) table_expression_;
    _CP_OPT(std::wstring) table_base_cell_address_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_named_expression);



}
}

#endif
