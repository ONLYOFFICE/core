
#include "table.h"

#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "common_attlists.h"

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

void table_table_attlist::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT( L"table:name",			table_name_);
	CP_XML_ATTR_OPT( L"table:style-name",	table_style_name_);
	CP_XML_ATTR_OPT( L"table:template-name", table_template_name_);

	if (table_protected_)
		CP_XML_ATTR_OPT( L"table:protection-key", table_protection_key_); 
	
	if (table_print_)
		CP_XML_ATTR_OPT( L"table:print-ranges", table_print_ranges_);
}
void table_table_row_attlist::serialize(CP_ATTR_NODE)
{
	if (table_number_rows_repeated_ > 1)
	{
		CP_XML_ATTR(L"table:number-rows-repeated", table_number_rows_repeated_); 
	}
    CP_XML_ATTR_OPT(L"table:style-name", table_style_name_);
    CP_XML_ATTR_OPT(L"table:default-cell-style-name", table_default_cell_style_name_);
    CP_XML_ATTR_OPT(L"table:visibility", table_visibility_);
}
void table_table_cell_attlist::serialize(CP_ATTR_NODE)
{
	if (table_number_columns_repeated_ > 1)
	{
		CP_XML_ATTR(L"table:number-columns-repeated", table_number_columns_repeated_);
	}
    CP_XML_ATTR_OPT(L"table:style-name", table_style_name_);
    CP_XML_ATTR_OPT(L"table:content-validation-name", table_content_validation_name_);
    CP_XML_ATTR_OPT(L"table:formula", table_formula_);

	if (common_value_and_type_attlist_)
		common_value_and_type_attlist_->serialize(CP_GET_XML_NODE());

    CP_XML_ATTR_OPT(L"table:protect", table_protect_);

}
void table_table_cell_attlist_extra::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"table:number-columns-spanned", table_number_columns_spanned_); 
    CP_XML_ATTR_OPT(L"table:number-rows-spanned", table_number_rows_spanned_); 
    CP_XML_ATTR_OPT(L"table:number-matrix-columns-spanned", table_number_matrix_columns_spanned_);
    CP_XML_ATTR_OPT(L"table:number-matrix-rows-spanned", table_number_matrix_rows_spanned_);    
}
void table_table_source_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR(L"table:mode", table_mode_ );
    CP_XML_ATTR_OPT(L"table:table-name", table_table_name_);

}
void table_linked_source_attlist::serialize(CP_ATTR_NODE)
{
	common_xlink_attlist_.serialize(CP_GET_XML_NODE());

    CP_XML_ATTR_OPT(L"table:filter-name", table_filter_name_);
    CP_XML_ATTR_OPT(L"table:filter-options", table_filter_options_);
    CP_XML_ATTR_OPT(L"table:refresh-delay", table_refresh_delay_);  
}
void table_table_column_attlist::serialize(CP_ATTR_NODE)
{
	if (table_number_columns_repeated_ > 1)
	{
		CP_XML_ATTR(L"table:number-columns-repeated", table_number_columns_repeated_);
	}
    CP_XML_ATTR_OPT(L"table:style-name", table_style_name_);
    CP_XML_ATTR_OPT(L"table:visibility", table_visibility_);  
    CP_XML_ATTR_OPT(L"table:default-cell-style-name", table_default_cell_style_name_);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const wchar_t * table_table_source::ns = L"table";
const wchar_t * table_table_source::name = L"table-source";

void table_table_source::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void table_table_source::add_child_element( office_element_ptr & child_element)
{
}
void table_table_source::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			table_table_source_attlist_.serialize(CP_GET_XML_NODE());
			table_linked_source_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

// table:table
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table::ns = L"table";
const wchar_t * table_table::name = L"table";

void table_table::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"table" == Ns && L"table-source" == Name)
    {
		CP_CREATE_ELEMENT(table_table_source_);
    } 
    else if ( (L"table" == Ns && L"table-column-group" == Name) ||
              (L"table" == Ns && L"table-columns" == Name) ||
              (L"table" == Ns && L"table-column" == Name) ||
              (L"table" == Ns && L"table-header-columns" == Name)
            )
    {
        table_columns_and_groups_.create_child_element(Ns, Name, getContext());
    }
    else if ((L"table" == Ns && L"table-row-group" == Name) ||
              (L"table" == Ns && L"table-rows" == Name) ||
              (L"table" == Ns && L"table-row" == Name) ||
              (L"table" == Ns && L"table-header-rows" == Name))
    {
        table_rows_and_groups_.create_child_element(Ns, Name, getContext());
    }
    else if CP_CHECK_NAME(L"table", L"shapes")
    {
  	    CP_CREATE_ELEMENT(table_shapes_);
    }
     else if CP_CHECK_NAME(L"table", L"named-expressions")
    {
  	    CP_CREATE_ELEMENT(table_named_expressions_);
    }
	 else
        CP_NOT_APPLICABLE_ELM();
}


void table_table::add_child_element(office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTableTableSource)
    {
		table_table_source_ = child_element;
    } 
    else if ( 	type == typeTableTableColumn ||
				type == typeTableTableColumns ||
				type == typeTableTableHeaderColumns || 
				type == typeTableTableColumnGroup)
    {
        table_columns_and_groups_.add_child_element(child_element, getContext());
    }
    else if (	type == typeTableTableRow ||
				type == typeTableTableRows ||
				type == typeTableTableHeaderRows ||
				type == typeTableTableRowGroup)
	{
        table_rows_and_groups_.add_child_element(child_element, getContext());
    }
    else if (type == typeTableShapes)
    {
  	    table_shapes_ = child_element;
    }
    else if (type == typeTableNamedExpressions)
    {
  	    table_named_expressions_ = child_element;
    }
	else if (type == typeCalcextConditionalFormats)
	{
		table_conditional_formats_ = child_element;
	}
}

void table_table::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			table_table_attlist_.serialize(CP_GET_XML_NODE());
		
			if (table_shapes_)table_shapes_->serialize(CP_XML_STREAM());
   
			table_columns_and_groups_.serialize(CP_XML_STREAM());
			table_rows_and_groups_.serialize(CP_XML_STREAM());
			
			if (table_named_expressions_)	table_named_expressions_->serialize(CP_XML_STREAM());
			if (table_conditional_formats_)	table_conditional_formats_->serialize(CP_XML_STREAM());
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-column
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_column::ns = L"table";
const wchar_t * table_table_column::name = L"table-column";

void table_table_column::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void table_table_column::add_child_element(office_element_ptr & child_element)
{
}
void table_table_column::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			table_table_column_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

// table:table-columns
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_columns::ns = L"table";
const wchar_t * table_table_columns::name = L"table-columns";

void table_table_columns::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"table-column")
    {
   	    CP_CREATE_ELEMENT(table_table_column_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}
void table_table_columns::add_child_element(office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTableTableColumn) 
		table_table_column_.push_back(child_element);
}
void table_table_columns::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			BOOST_FOREACH(const office_element_ptr & elm, table_table_column_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}

// table:table-header-columns
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_header_columns::ns = L"table";
const wchar_t * table_table_header_columns::name = L"table-header-columns";

void table_table_header_columns::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"table-column")
    {
		 CP_CREATE_ELEMENT(table_table_column_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}
void table_table_header_columns::add_child_element(office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTableTableColumn) 
		table_table_column_.push_back(child_element);
}
void table_table_header_columns::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			BOOST_FOREACH(const office_element_ptr & elm, table_table_column_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}

// table-columns
//////////////////////////////////////////////////////////////////////////////////////////////////
void table_columns::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if CP_CHECK_NAME(L"table", L"table-columns")
    {
		CP_CREATE_ELEMENT_SIMPLE(table_table_columns_);
    }
    else if CP_CHECK_NAME(L"table", L"table-column")
    {
        CP_CREATE_ELEMENT_SIMPLE(table_table_column_);                
    } 
    else
        not_applicable_element(L"table-columns",  Ns, Name);
}
void table_columns::add_child_element(office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTableTableColumns) 
	{
		if (table_table_columns_) 
		{
			//error???
		}
		table_table_columns_ = child_element;
	}
	else if (type == typeTableTableColumn) 
		table_table_column_.push_back(child_element);
}
void table_columns::serialize(std::wostream & _Wostream)
{
	if (table_table_columns_)	table_table_columns_->serialize(_Wostream);
	
	BOOST_FOREACH(const office_element_ptr & elm, table_table_column_)
	{
		elm->serialize(_Wostream);
	}
}
// table-columns-no-group
//////////////////////////////////////////////////////////////////////////////////////////////////

//const wchar_t * table_columns_no_group::ns = L"table";
//const wchar_t * table_columns_no_group::name = L"table-columns";
table_columns_no_group::table_columns_no_group(odf_conversion_context * _Context) : was_header_(false) 
{
	Context = _Context;
};

void table_columns_no_group::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (CP_CHECK_NAME(L"table", L"table-columns") || CP_CHECK_NAME(L"table", L"table-column"))
    {
        if (!was_header_)
            table_columns_1_.create_child_element(Ns, Name, Context);
        else
            table_columns_2_.create_child_element( Ns, Name, Context);

    } 
    else if CP_CHECK_NAME(L"table", L"table-header-columns")
    {
        was_header_ = true;
		CP_CREATE_ELEMENT(table_table_header_columns_);
    }
    else
        not_applicable_element(L"table-columns-no-group", Ns, Name);
}
void table_columns_no_group::add_child_element(office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTableTableColumns || type == typeTableTableColumn) 
	{
        if (!was_header_)
            table_columns_1_.add_child_element(child_element);
        else
            table_columns_2_.add_child_element(child_element);
	}
	else if (type == typeTableTableHeaderColumns) 
	{
        was_header_ = true;
		table_table_header_columns_ = child_element;
	}
}
void table_columns_no_group::serialize(std::wostream & _Wostream)
{
  //  CP_XML_WRITER(_Wostream)
  //  {
		//CP_XML_NODE_SIMPLE()
  //      {
			table_columns_1_.serialize(_Wostream);
 			
			if (table_table_header_columns_)
				table_table_header_columns_->serialize(_Wostream);
			
			table_columns_2_.serialize(_Wostream);			
	//	}
	//}
}
_CP_PTR(table_columns_no_group) table_columns_no_group::create(odf_conversion_context * Context)
{
    return boost::make_shared<table_columns_no_group>(Context);
}

// table:table-column-group
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_column_group::ns = L"table";
const wchar_t * table_table_column_group::name = L"table-column-group";

void table_table_column_group::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    table_columns_and_groups_.create_child_element(Ns, Name, getContext());
}
void table_table_column_group::add_child_element(office_element_ptr & child_element)
{
	table_columns_and_groups_.add_child_element(child_element,getContext());
}
void table_table_column_group::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR( L"table:display",	table_table_column_group_attlist_.table_display_);

			table_columns_and_groups_.serialize(CP_XML_STREAM());	
		}
	}
}
// table-columns-and-groups
//////////////////////////////////////////////////////////////////////////////////////////////////

table_columns_and_groups::table_columns_and_groups()
{
}


void table_columns_and_groups::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if (CP_CHECK_NAME(L"table", L"table-column-group"))
    {
		CP_CREATE_ELEMENT_SIMPLE(content_);
    }
    else if (CP_CHECK_NAME(L"table", L"table-columns") ||
        CP_CHECK_NAME(L"table", L"table-column") ||
        CP_CHECK_NAME(L"table", L"table-header-columns"))
    {
        _CP_PTR(table_columns_no_group) elm = table_columns_no_group::create(Context);

        elm->create_child_element(Ns, Name);
        content_.push_back(elm);
    }
    else
        not_applicable_element(L"table-columns-and-groups", Ns, Name);

}
void table_columns_and_groups::add_child_element(office_element_ptr & child_element, odf_conversion_context * Context)
{
	ElementType type = child_element->get_type();

    if (type == typeTableTableColumnGroup) 
	{
		content_.push_back(child_element);
	}
    else if (type == typeTableTableColumns ||
			type == typeTableTableColumn ||
			type == typeTableTableHeaderColumns)
    {
       _CP_PTR(table_columns_no_group) elm = table_columns_no_group::create(Context);

        elm->add_child_element(child_element);
        content_.push_back(elm);
	}
	else
	{
		not_applicable_element(L"table-columns-and-groups", L"", L"");
	}
}
void table_columns_and_groups::serialize(std::wostream & _Wostream)
{
	BOOST_FOREACH(const office_element_ptr & elm, content_)
	{
		elm->serialize(_Wostream);
	}
}
// table-table-cell-content
//////////////////////////////////////////////////////////////////////////////////////////////////

void table_table_cell_content::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
	CP_CREATE_ELEMENT_SIMPLE(text_content_);
}
void table_table_cell_content::add_child_element( office_element_ptr & child_element)
{
	text_content_.push_back(child_element);
}
void table_table_cell_content::serialize(std::wostream & _Wostream)
{
	BOOST_FOREACH(const office_element_ptr & elm, text_content_)
	{
		if (elm) elm->serialize(_Wostream);
	}
}
// table:table-cell
// table-table-cell
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_cell::ns = L"table";
const wchar_t * table_table_cell::name = L"table-cell";

void table_table_cell::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    table_table_cell_content_.create_child_element(Ns, Name, getContext());
}

void table_table_cell::add_child_element(office_element_ptr & child_element)
{
	table_table_cell_content_.add_child_element(child_element);
}
void table_table_cell::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			table_table_cell_attlist_.serialize(CP_GET_XML_NODE());
			table_table_cell_attlist_extra_.serialize(CP_GET_XML_NODE());
				
			table_table_cell_content_.serialize(CP_XML_STREAM());
		}
	}	
}
// table:covered-table-cell
// table-covered-table-cell
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_covered_table_cell::ns = L"table";
const wchar_t * table_covered_table_cell::name = L"covered-table-cell";

void table_covered_table_cell::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
	empty_ = false;
    table_table_cell_content_.create_child_element( Ns, Name, getContext());
}
void table_covered_table_cell::add_child_element(office_element_ptr & child_element)
{
	empty_ = false;
	table_table_cell_content_.add_child_element(child_element);
}
void table_covered_table_cell::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			table_table_cell_attlist_.serialize(CP_GET_XML_NODE());
				
			table_table_cell_content_.serialize(CP_XML_STREAM());
		}
	}	
}
// table:table-row
// table-table-row
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_row::ns = L"table";
const wchar_t * table_table_row::name = L"table-row";

void table_table_row::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if ( CP_CHECK_NAME(L"table", L"table-cell") || 
         CP_CHECK_NAME(L"table", L"table-covered-cell") )
    {
		CP_CREATE_ELEMENT(content_);
    }
    else
        CP_NOT_APPLICABLE_ELM();    
}
void table_table_row::add_child_element(office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTableTableCell || type == typeTableCoveredTableCell )
		content_.push_back(child_element);
}
void table_table_row::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			table_table_row_attlist_.serialize(CP_GET_XML_NODE());
				
			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}	
}
// table:table-rows
// table-table-rows
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_rows::ns = L"table";
const wchar_t * table_table_rows::name = L"table-rows";

void table_table_rows::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(table_table_row_);
}
void table_table_rows::add_child_element(office_element_ptr & child_element)
{
	table_table_row_.push_back(child_element);    
}
void table_table_rows::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			BOOST_FOREACH(const office_element_ptr & elm, table_table_row_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}	
}
// table:table-header-rows
// table-table-header-rows
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_header_rows::ns = L"table";
const wchar_t * table_table_header_rows::name = L"table-header-rows";

void table_table_header_rows::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"table-row")
    {
		CP_CREATE_ELEMENT(table_table_row_);
    }
    else
        CP_NOT_APPLICABLE_ELM();    
}
void table_table_header_rows::add_child_element(office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTableTableRow)
		table_table_row_.push_back(child_element);    
}
void table_table_header_rows::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			BOOST_FOREACH(const office_element_ptr & elm, table_table_row_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}	
}
// table-rows
//////////////////////////////////////////////////////////////////////////////////////////////////
void table_rows::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if CP_CHECK_NAME(L"table", L"table-rows")
    {
		CP_CREATE_ELEMENT_SIMPLE(table_table_rows_);
    } 
    else if CP_CHECK_NAME(L"table", L"table-row")
    {
		CP_CREATE_ELEMENT_SIMPLE(table_table_row_);
    }
    else
    {
        not_applicable_element(L"table-rows", Ns, Name);        
    }
}
void table_rows::add_child_element(office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTableTableRows)
		table_table_rows_ = child_element;    
   
	else if (type == typeTableTableRow)
		table_table_row_.push_back(child_element);    
}
void table_rows::serialize(std::wostream & _Wostream)
{
	if (table_table_rows_)
		table_table_rows_->serialize(_Wostream);
	
	BOOST_FOREACH(const office_element_ptr & elm, table_table_row_)
	{
		elm->serialize(_Wostream);
	}
}
// table-rows-no-group
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_rows_no_group::ns = L"table";
const wchar_t * table_rows_no_group::name = L"table-rows-no-group";

_CP_PTR(table_rows_no_group) table_rows_no_group::create(odf_conversion_context * Context)
{
    return boost::make_shared<table_rows_no_group>(Context);
}

table_rows_no_group::table_rows_no_group(odf_conversion_context * _Context) : was_header_(false)
{
	Context = _Context;
};

void table_rows_no_group::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (CP_CHECK_NAME(L"table", L"table-rows") || CP_CHECK_NAME(L"table", L"table-row"))
    {
        if (!was_header_)
            table_rows_1_.create_child_element(Ns, Name, Context);
        else
            table_rows_2_.create_child_element(Ns, Name, Context);
    } 
    else if CP_CHECK_NAME(L"table", L"table-header-rows")
    {
        was_header_ = true;
		CP_CREATE_ELEMENT(table_table_header_rows_);
    }
    else
        not_applicable_element(L"table-rows-no-group",Ns, Name);
}
void table_rows_no_group::add_child_element(office_element_ptr & child_element)
{
	ElementType type = child_element->get_type();

    if (type == typeTableTableRows || type == typeTableTableRow)
	{
       if (!was_header_)
            table_rows_1_.add_child_element(child_element);
        else
            table_rows_2_.add_child_element(child_element);
	}   
	else if (type == typeTableTableHeaderRows)
	{
        was_header_ = true;
		if (table_table_header_rows_ == NULL)	
			table_table_header_rows_ = child_element;    
	}	
}
void table_rows_no_group::serialize(std::wostream & _Wostream)
{
  //  CP_XML_WRITER(_Wostream)
  //  {
		//CP_XML_NODE_SIMPLE()
  //      {
			table_rows_1_.serialize(_Wostream);
			
			if (table_table_header_rows_)
				table_table_header_rows_->serialize(_Wostream);
			
			table_rows_2_.serialize(_Wostream);
/*		}
	}*/	
}
// table-rows-and-groups
//////////////////////////////////////////////////////////////////////////////////////////////////

table_rows_and_groups::table_rows_and_groups()
{
}

void table_rows_and_groups::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if (L"table" == Ns && L"table-row-group" == Name)
    {
		CP_CREATE_ELEMENT_SIMPLE(content_);
    } 
    else if (L"table" == Ns && (L"table-rows" == Name || L"table-row" == Name || L"table-header-rows" == Name) )
    {
        _CP_PTR(table_rows_no_group) elm = table_rows_no_group::create(Context);
        elm->create_child_element(Ns, Name);
        content_.push_back(elm);
   }
    else
        not_applicable_element(L"table-rows-and-groups", Ns, Name);
}
void table_rows_and_groups::add_child_element(office_element_ptr & child_element, odf_conversion_context * Context)
{
	ElementType type = child_element->get_type();

	if (type == typeTableTableRowGroup)
	{
		content_.push_back(child_element); 
	}
    else if (type == typeTableTableRows || type == typeTableTableRow  || type == typeTableTableHeaderRows)
	{
        _CP_PTR(table_rows_no_group) elm = table_rows_no_group::create(Context);
        elm->add_child_element(child_element);
        content_.push_back(elm);
	}
}
void table_rows_and_groups::serialize(std::wostream & _Wostream)
{
	BOOST_FOREACH(const office_element_ptr & elm, content_)
	{
		elm->serialize(_Wostream);
	}
}
// table:table-row-group
// table-table-row-group
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_row_group::ns = L"table";
const wchar_t * table_table_row_group::name = L"table-row-group";


void table_table_row_group::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    table_rows_and_groups_.create_child_element(Ns, Name, getContext());
}

void table_table_row_group::add_child_element(office_element_ptr & child_element)
{
    table_rows_and_groups_.add_child_element(child_element, getContext());
}
 void table_table_row_group::serialize(std::wostream & _Wostream)
 {
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR( L"table:display",	table_table_row_group_attlist_.table_display_);
			
			table_rows_and_groups_.serialize(CP_XML_STREAM());
		}
	}	
 }
///////////


// table:shapes
// table-shapes
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_shapes::ns = L"table";
const wchar_t * table_shapes::name = L"shapes";

void table_shapes::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
	create_element(Ns,Name,content_,getContext());
}

void table_shapes::add_child_element(office_element_ptr & child_element)
{
    content_.push_back(child_element);
}

void table_shapes::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
        {
			BOOST_FOREACH(const office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}

}
}
