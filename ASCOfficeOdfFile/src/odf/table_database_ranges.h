#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "datatypes/common_attlists.h"

#include "../docx/xlsxconversioncontext.h"

namespace cpdoccore { 
	using namespace odf_types;
namespace odf_reader {

//  table:named-expressions
class table_database_ranges : public office_element_impl<table_database_ranges>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTableDatabaseRanges;

    CPDOCCORE_DEFINE_VISITABLE()

    virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element	( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_database_ranges);

//  table:named-range
class table_database_range: public office_element_impl<table_database_range>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTableDatabaseRange;

    CPDOCCORE_DEFINE_VISITABLE()

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

    _CP_OPT(std::wstring)		table_name_;
    _CP_OPT(std::wstring)		table_target_range_address_;
	_CP_OPT(Bool)				table_display_filter_buttons_;
	_CP_OPT(Bool)				table_contains_header_;	
	_CP_OPT(std::wstring)		table_orientation_;
//table:refresh-delay

	office_element_ptr_array	content_;
//"table-database-source-sql"
//"table-database-source-table"
//"table-database-source-query"
};

CP_REGISTER_OFFICE_ELEMENT2(table_database_range);

//  table:sort
class table_sort: public office_element_impl<table_sort>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTableSort;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes ){}
    virtual void add_child_element	( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

	office_element_ptr_array	table_sort_by_;
//attr
	//table:algorithm
	//table:country
	//table:language
	//table:case-sensitive
};

CP_REGISTER_OFFICE_ELEMENT2(table_sort);

//  table:sort-by
class table_sort_by: public office_element_impl<table_sort_by>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type		= typeTableSortBy;

    CPDOCCORE_DEFINE_VISITABLE()

	virtual void add_attributes		( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element	( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name){}

	int						table_field_number_;
	_CP_OPT(std::wstring)	table_data_type_;
	_CP_OPT(std::wstring)	table_order_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_sort_by);
}
}
