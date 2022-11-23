/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "table.h"

#include <boost/make_shared.hpp>

#include <xml/xmlchar.h>
#include <xml/attributes.h>
#include <xml/simple_xml_writer.h>

#include "../../DataTypes/common_attlists.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

using xml::xml_char_wc;

void table_table_attlist::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT_ENCODE_STRING( L"table:name", table_name_);
	CP_XML_ATTR_OPT( L"table:style-name", table_style_name_);
	CP_XML_ATTR_OPT_ENCODE_STRING( L"table:template-name", table_template_name_);

	if (table_protected_ && table_protected_->get())
	{
		CP_XML_ATTR_OPT( L"table:protected", table_protected_); 
		CP_XML_ATTR_OPT( L"table:protection-key", table_protection_key_); 
		CP_XML_ATTR_OPT( L"table:protection-digest-algorithm", table_protection_key_digest_algorithm_); 
	}	

	CP_XML_ATTR_OPT_ENCODE_STRING( L"table:print-ranges", table_print_ranges_);
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
	CP_XML_ATTR_OPT_ENCODE_STRING(L"table:formula", table_formula_);

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
	CP_XML_ATTR_OPT_ENCODE_STRING(L"table:table-name", table_table_name_);

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

const wchar_t * table_table_protection::ns = L"loext"; //table ???
const wchar_t * table_table_protection::name = L"table-protection";

void table_table_protection::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR(L"loext:select-protected-cells", select_protected_cells);
			CP_XML_ATTR(L"loext:select-unprotected-cells", select_unprotected_cells);

			CP_XML_ATTR_OPT(L"loext:insert-columns", insert_columns);
			CP_XML_ATTR_OPT(L"loext:insert-rows", insert_rows);

			CP_XML_ATTR_OPT(L"loext:delete-columns", delete_columns);
			CP_XML_ATTR_OPT(L"loext:delete-rows", delete_rows);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////

const wchar_t * table_table_source::ns = L"table";
const wchar_t * table_table_source::name = L"table-source";

void table_table_source::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void table_table_source::add_child_element( const office_element_ptr & child_element)
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

void table_table::create_child_element(const std::wstring & Ns, const std::wstring & Name)
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
    else if CP_CHECK_NAME(L"office", L"forms")
    {
  	    CP_CREATE_ELEMENT(office_forms_);
    }
	else if CP_CHECK_NAME(L"table", L"named-expressions")
    {
  	    CP_CREATE_ELEMENT(table_named_expressions_);
    }
	 else
        CP_NOT_APPLICABLE_ELM();
}


void table_table::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableTableSource)
    {
		table_table_source_ = child_element;
    } 
    else if ( 	type_ == typeTableTableColumn ||
				type_ == typeTableTableColumns ||
				type_ == typeTableTableHeaderColumns || 
				type_ == typeTableTableColumnGroup)
    {
        table_columns_and_groups_.add_child_element(child_element, getContext());
    }
    else if (	type_ == typeTableTableRow ||
				type_ == typeTableTableRows ||
				type_ == typeTableTableHeaderRows ||
				type_ == typeTableTableRowGroup)
	{
        table_rows_and_groups_.add_child_element(child_element, getContext());
    }
    else if (type_ == typeTableShapes)
    {
  	    table_shapes_ = child_element;
    }
	else if (type_ == typeOfficeForms)
	{
		office_forms_ = child_element;
	}
    else if (type_ == typeTableNamedExpressions)
    {
  	    table_named_expressions_ = child_element;
    }
	else if (type_ == typeCalcextConditionalFormats)
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
			attlist_.serialize(CP_GET_XML_NODE());
			
			if (table_protection_)table_protection_->serialize(CP_XML_STREAM());
		
			if (office_forms_)office_forms_->serialize(CP_XML_STREAM());

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

void table_table_column::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void table_table_column::add_child_element( const office_element_ptr & child_element)
{
}
void table_table_column::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

// table:table-columns
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_columns::ns = L"table";
const wchar_t * table_table_columns::name = L"table-columns";

void table_table_columns::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"table-column")
    {
   	    CP_CREATE_ELEMENT(content_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}
void table_table_columns::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableTableColumn) 
		content_.push_back(child_element);
}
void table_table_columns::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

// table:table-header-columns
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_header_columns::ns = L"table";
const wchar_t * table_table_header_columns::name = L"table-header-columns";

void table_table_header_columns::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"table-column")
    {
		 CP_CREATE_ELEMENT(content_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}
void table_table_header_columns::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableTableColumn) 
		content_.push_back(child_element);
}
void table_table_header_columns::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

// table-columns
//////////////////////////////////////////////////////////////////////////////////////////////////
void table_columns::create_child_element( const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context)
{
    if CP_CHECK_NAME(L"table", L"table-columns")
    {
		CP_CREATE_ELEMENT_SIMPLE(table_table_columns_);
    }
    else if CP_CHECK_NAME(L"table", L"table-column")
    {
        CP_CREATE_ELEMENT_SIMPLE(content_);                
    } 
    else
        not_applicable_element(L"table-columns",  Ns, Name);
}
void table_columns::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableTableColumns) 
	{
		if (table_table_columns_) 
		{
			//error???
		}
		table_table_columns_ = child_element;
	}
	else if (type_ == typeTableTableColumn) 
		content_.push_back(child_element);
}
void table_columns::serialize(std::wostream & strm)
{
	if (table_table_columns_)	table_table_columns_->serialize(strm);
	
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->serialize(strm);
	}
}
// table-columns-no-group
//////////////////////////////////////////////////////////////////////////////////////////////////

const wchar_t * table_columns_no_group::ns = L"table";
const wchar_t * table_columns_no_group::name = L"table-columns";
table_columns_no_group::table_columns_no_group(odf_conversion_context * _Context) : was_header_(false) 
{
	Context = _Context;
};

void table_columns_no_group::create_child_element(  const std::wstring & Ns, const std::wstring & Name)
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
void table_columns_no_group::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableTableColumns || type_ == typeTableTableColumn) 
	{
        if (!was_header_)
            table_columns_1_.add_child_element(child_element);
        else
            table_columns_2_.add_child_element(child_element);
	}
	else if (type_ == typeTableTableHeaderColumns) 
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

void table_table_column_group::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    table_columns_and_groups_.create_child_element(Ns, Name, getContext());
}
void table_table_column_group::add_child_element( const office_element_ptr & child_element)
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


void table_columns_and_groups::create_child_element(const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context)
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
void table_columns_and_groups::add_child_element( const office_element_ptr & child_element, odf_conversion_context * Context)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableTableColumnGroup) 
	{
		content_.push_back(child_element);
	}
    else if (type_ == typeTableTableColumns ||
			type_ == typeTableTableColumn ||
			type_ == typeTableTableHeaderColumns)
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
void table_columns_and_groups::serialize(std::wostream & strm)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->serialize(strm);
	}
}
// table:table-cell
// table-table-cell
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_cell::ns = L"table";
const wchar_t * table_table_cell::name = L"table-cell";

void table_table_cell::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void table_table_cell::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void table_table_cell::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			attlist_.serialize(CP_GET_XML_NODE());
			attlist_extra_.serialize(CP_GET_XML_NODE());
				
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}	
}
// table:covered-table-cell
// table-covered-table-cell
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_covered_table_cell::ns = L"table";
const wchar_t * table_covered_table_cell::name = L"covered-table-cell";

void table_covered_table_cell::create_child_element(  const std::wstring & Ns, const std::wstring & Name)
{
	empty_ = false;
	CP_CREATE_ELEMENT(content_);
}
void table_covered_table_cell::add_child_element( const office_element_ptr & child_element)
{
	empty_ = false;
	content_.push_back(child_element);
}
void table_covered_table_cell::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			attlist_.serialize(CP_GET_XML_NODE());
				
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}	
}
// table:table-row
// table-table-row
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_row::ns = L"table";
const wchar_t * table_table_row::name = L"table-row";

void table_table_row::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    if ( CP_CHECK_NAME(L"table", L"table-cell") || 
         CP_CHECK_NAME(L"table", L"table-covered-cell") )
    {
		CP_CREATE_ELEMENT(content_);
    }
    else
        CP_NOT_APPLICABLE_ELM();    
}
void table_table_row::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableTableCell || type_ == typeTableCoveredTableCell )
		content_.push_back(child_element);
}
void table_table_row::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			attlist_.serialize(CP_GET_XML_NODE());
				
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}	
}
// table:table-rows
// table-table-rows
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_rows::ns = L"table";
const wchar_t * table_table_rows::name = L"table-rows";

void table_table_rows::create_child_element(  const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void table_table_rows::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);    
}
void table_table_rows::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(strm);
			}
		}
	}	
}
// table:table-header-rows
// table-table-header-rows
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_header_rows::ns = L"table";
const wchar_t * table_table_header_rows::name = L"table-header-rows";

void table_table_header_rows::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"table-row")
    {
		CP_CREATE_ELEMENT(content_);
    }
    else
        CP_NOT_APPLICABLE_ELM();    
}
void table_table_header_rows::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableTableRow)
		content_.push_back(child_element);    
}
void table_table_header_rows::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}	
}
// table-rows
//////////////////////////////////////////////////////////////////////////////////////////////////
void table_rows::create_child_element(const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context)
{
    if CP_CHECK_NAME(L"table", L"table-rows")
    {
		CP_CREATE_ELEMENT_SIMPLE(table_table_rows_);
    } 
    else if CP_CHECK_NAME(L"table", L"table-row")
    {
		CP_CREATE_ELEMENT_SIMPLE(content_);
    }
    else
    {
        not_applicable_element(L"table-rows", Ns, Name);        
    }
}
void table_rows::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableTableRows)
		table_table_rows_ = child_element;    
   
	else if (type_ == typeTableTableRow)
		content_.push_back(child_element);    
}
void table_rows::serialize(std::wostream & strm)
{
	if (table_table_rows_)
		table_table_rows_->serialize(strm);
	
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->serialize(strm);
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

void table_rows_no_group::create_child_element( const std::wstring & Ns, const std::wstring & Name)
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
void table_rows_no_group::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTableTableRows || type_ == typeTableTableRow)
	{
       if (!was_header_)
            table_rows_1_.add_child_element(child_element);
        else
            table_rows_2_.add_child_element(child_element);
	}   
	else if (type_ == typeTableTableHeaderRows)
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

void table_rows_and_groups::create_child_element( const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context)
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
void table_rows_and_groups::add_child_element( const office_element_ptr & child_element, odf_conversion_context * Context)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

	if (type_ == typeTableTableRowGroup)
	{
		content_.push_back(child_element); 
	}
    else if (type_ == typeTableTableRows || type_ == typeTableTableRow  || type_ == typeTableTableHeaderRows)
	{
        _CP_PTR(table_rows_no_group) elm = table_rows_no_group::create(Context);
        elm->add_child_element(child_element);
        content_.push_back(elm);
	}
}
void table_rows_and_groups::serialize(std::wostream & strm)
{
	for (size_t i = 0; i < content_.size(); i++)
	{
		content_[i]->serialize(strm);
	}
}
// table:table-row-group
// table-table-row-group
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_row_group::ns = L"table";
const wchar_t * table_table_row_group::name = L"table-row-group";


void table_table_row_group::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    table_rows_and_groups_.create_child_element(Ns, Name, getContext());
}

void table_table_row_group::add_child_element( const office_element_ptr & child_element)
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

void table_shapes::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	create_element(Ns,Name,content_,getContext());
}

void table_shapes::add_child_element( const office_element_ptr & child_element)
{
    content_.push_back(child_element);
}

void table_shapes::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_content_validations::ns = L"table";
const wchar_t * table_content_validations::name = L"content-validations";

void table_content_validations::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
void table_content_validations::add_child_element( const office_element_ptr & child_element)
{
    content_.push_back(child_element);
}
void table_content_validations::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_content_validation::ns = L"table";
const wchar_t * table_content_validation::name = L"content-validation";

void table_content_validation::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
void table_content_validation::add_child_element( const office_element_ptr & child_element)
{
    content_.push_back(child_element);
}
void table_content_validation::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:name", table_name_);
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:condition", table_condition_);
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:display-list", table_display_list_);
			CP_XML_ATTR_OPT(L"table:allow-empty-cell", table_allowempty_cell_);
			CP_XML_ATTR_OPT_ENCODE_STRING(L"table:base-cell-address", table_base_cell_address_);
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_error_message::ns = L"table";
const wchar_t * table_error_message::name = L"error-message";

void table_error_message::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
void table_error_message::add_child_element( const office_element_ptr & child_element)
{
    content_.push_back(child_element);
}
void table_error_message::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"table:title",			table_title_);
			CP_XML_ATTR_OPT(L"table:message-type",	table_message_type_);
			CP_XML_ATTR_OPT(L"table:display",		table_display_);
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_help_message::ns = L"table";
const wchar_t * table_help_message::name = L"help-message";

void table_help_message::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
void table_help_message::add_child_element( const office_element_ptr & child_element)
{
    content_.push_back(child_element);
}
void table_help_message::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"table:title",			table_title_);
			CP_XML_ATTR_OPT(L"table:message-type",	table_message_type_);
			CP_XML_ATTR_OPT(L"table:display",		table_display_);
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
}
}
