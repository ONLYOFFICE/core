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
#include "text_elements.h"

#include <boost/make_shared.hpp>

#include "serialize_elements.h"

#include "odfcontext.h"
#include "odf_document.h"


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

// table:table-attlist
void table_table_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"table:name",			table_name_);
    CP_APPLY_ATTR(L"table:style-name",		table_style_name_);
    CP_APPLY_ATTR(L"table:template-name",	table_template_name_);

    CP_APPLY_ATTR(L"table:protected",		table_protected_, false);
    CP_APPLY_ATTR(L"table:protection-key",	table_protection_key_); 
	CP_APPLY_ATTR(L"table:protection-key-digest-algorithm",	table_protection_key_digest_algorithm_);

    CP_APPLY_ATTR(L"table:print",			table_print_, true);
    CP_APPLY_ATTR(L"table:print-ranges",	table_print_ranges_);
	
	CP_APPLY_ATTR(L"table:use-first-row-styles",		table_use_first_row_styles_,false);
	CP_APPLY_ATTR(L"table:use-banding-rows-styles",		table_use_banding_rows_styles_,false);
	CP_APPLY_ATTR(L"table:use-banding-columns-styles",	table_use_banding_columns_styles_,false);
	CP_APPLY_ATTR(L"table:use-first-column-styles",		table_use_first_column_styles_,false);

	CP_APPLY_ATTR(L"table:is-sub-table",	table_is_sub_table_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-row-attlist
void table_table_row_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"table:number-rows-repeated", table_number_rows_repeated_, (unsigned int)1/*0*/); 
    CP_APPLY_ATTR(L"table:style-name", table_style_name_);
    CP_APPLY_ATTR(L"table:default-cell-style-name", table_default_cell_style_name_);
    CP_APPLY_ATTR(L"table:visibility", table_visibility_, table_visibility(table_visibility::Visible));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-cell-attlist
void table_table_cell_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"table:number-columns-repeated", table_number_columns_repeated_, (unsigned int)1/*0*/);
    CP_APPLY_ATTR(L"table:style-name",				table_style_name_);
    CP_APPLY_ATTR(L"table:content-validation-name", table_content_validation_name_);
    CP_APPLY_ATTR(L"table:formula",					table_formula_);
    CP_APPLY_ATTR(L"table:protect",					table_protect_, (bool)table_protect_);

    common_value_and_type_attlist_.add_attributes(Attributes);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-cell-attlist-extra
void table_table_cell_attlist_extra::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"table:number-columns-spanned", table_number_columns_spanned_, (unsigned int)/*1*/1); 
    CP_APPLY_ATTR(L"table:number-rows-spanned", table_number_rows_spanned_, (unsigned int)1); 
    CP_APPLY_ATTR(L"table:number-matrix-columns-spanned", table_number_matrix_columns_spanned_);
    CP_APPLY_ATTR(L"table:number-matrix-rows-spanned", table_number_matrix_rows_spanned_);    
}

// table-table-source-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////
void table_table_source_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"table:mode", table_mode_, table_mode( table_mode::CopyAll ) );
    CP_APPLY_ATTR(L"table:table-name", table_table_name_);

}

void table_linked_source_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	xlink_attlist_.add_attributes(Attributes);

    CP_APPLY_ATTR(L"table:filter-name", table_filter_name_);
    CP_APPLY_ATTR(L"table:filter-options", table_filter_options_);
    CP_APPLY_ATTR(L"table:refresh-delay", table_refresh_delay_);  
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-source
const wchar_t * table_table_source::ns = L"table";
const wchar_t * table_table_source::name = L"table-source";

void table_table_source::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    table_table_source_attlist_.add_attributes(Attributes);
    table_linked_source_attlist_.add_attributes(Attributes);
}

void table_table_source::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-protection
const wchar_t * table_table_protection::ns = L"loext"; //?? table odf 1.3
const wchar_t * table_table_protection::name = L"table-protection";

void table_table_protection::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"loext:select-protected-cells",	select_protected_cells);
	CP_APPLY_ATTR(L"loext:select-unprotected-cells",select_unprotected_cells);
	CP_APPLY_ATTR(L"loext:insert-columns",			insert_columns);
	CP_APPLY_ATTR(L"loext:insert-rows",				insert_rows);
	CP_APPLY_ATTR(L"loext:delete-columns",			delete_columns);
	CP_APPLY_ATTR(L"loext:delete-rows",				delete_rows);
	//CP_APPLY_ATTR(L"loext:format-columns",		format_columns);
	//CP_APPLY_ATTR(L"loext:format-rows",			format_rows);
	//CP_APPLY_ATTR(L"loext:format-cells",			format_cells);
}
//---------------------------------------------------------------------------------------------------------
// table:table
const wchar_t * table_sub_table::ns = L"table";
const wchar_t * table_sub_table::name = L"sub-table";
//---------------------------------------------------------------------------------------------------------
// table:table
const wchar_t * table_table::ns = L"table";
const wchar_t * table_table::name = L"table";

void table_table::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"table:style-name",	element_style_name);

	attlist_.add_attributes(Attributes);
}

void table_table::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
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
        table_columns_and_groups_.add_child_element(Reader, Ns, Name, getContext());
    }
    else if ((L"table" == Ns && L"table-row-group" == Name) ||
              (L"table" == Ns && L"table-rows" == Name) ||
              (L"table" == Ns && L"table-row" == Name) ||
              (L"table" == Ns && L"table-header-rows" == Name)||
              (L"text" == Ns && L"soft-page-break" == Name))
    {
        table_rows_and_groups_.add_child_element(Reader, Ns, Name, getContext());
    }
    else if CP_CHECK_NAME(L"table", L"shapes")
    {
        CP_CREATE_ELEMENT(table_shapes_);    
    }
    else if ( L"table-protection" == Name)  //ns = loext or table
    {
        CP_CREATE_ELEMENT(table_protection_);    
    }
	else if (	CP_CHECK_NAME(L"table", L"named-expressions") || 
				CP_CHECK_NAME(L"table", L"named-range") )
	{
        CP_CREATE_ELEMENT(table_named_);    
	}
	else if CP_CHECK_NAME(L"calcext", L"conditional-formats")
    {
        CP_CREATE_ELEMENT(conditional_formats_);    
    }
	else if CP_CHECK_NAME(L"office", L"forms")
    {
        CP_CREATE_ELEMENT(office_forms_);    
    }
	else 
        CP_NOT_APPLICABLE_ELM();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
void table_table_column_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"table:number-columns-repeated", table_number_columns_repeated_, (unsigned int)1/*0*/);
    CP_APPLY_ATTR(L"table:style-name", table_style_name_);
    CP_APPLY_ATTR(L"table:visibility", table_visibility_, table_visibility(table_visibility::Visible));  
    CP_APPLY_ATTR(L"table:default-cell-style-name", table_default_cell_style_name_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-column
const wchar_t * table_table_column::ns = L"table";
const wchar_t * table_table_column::name = L"table-column";

void table_table_column::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    table_table_column_attlist_.add_attributes(Attributes);
}

void table_table_column::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-columns
const wchar_t * table_table_columns::ns = L"table";
const wchar_t * table_table_columns::name = L"table-columns";

void table_table_columns::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void table_table_columns::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"table-column")
    {
         CP_CREATE_ELEMENT(table_table_column_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-header-columns
const wchar_t * table_table_header_columns::ns = L"table";
const wchar_t * table_table_header_columns::name = L"table-header-columns";

void table_table_header_columns::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void table_table_header_columns::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"table-column")
    {
         CP_CREATE_ELEMENT(table_table_column_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:columns
void table_columns::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
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
        not_applicable_element(L"table-columns", Reader, Ns, Name);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:columns-no-group
table_columns_no_group::table_columns_no_group() : was_header_(false) 
{
};

void table_columns_no_group::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
{
    if (CP_CHECK_NAME(L"table", L"table-columns") || CP_CHECK_NAME(L"table", L"table-column"))
    {
        if (!was_header_)
            table_columns_1_.add_child_element(Reader, Ns, Name, Context);
        else
            table_columns_2_.add_child_element(Reader, Ns, Name, Context);

    } 
    else if CP_CHECK_NAME(L"table", L"table-header-columns")
    {
        was_header_ = true;
        CP_CREATE_ELEMENT_SIMPLE(table_table_header_columns_);            
    }
    else
        not_applicable_element(L"table-columns-no-group", Reader, Ns, Name);
}

_CP_PTR(table_columns_no_group) table_columns_no_group::create()
{
    return boost::make_shared<table_columns_no_group>();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-column-group-attlist
void table_table_column_group_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"table:display", table_display_, true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-column-group
const wchar_t * table_table_column_group::ns = L"table";
const wchar_t * table_table_column_group::name = L"table-column-group";

void table_table_column_group::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes(Attributes);
}

void table_table_column_group::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    table_columns_and_groups_.add_child_element(Reader, Ns, Name, getContext());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:columns-and-groups
table_columns_and_groups::table_columns_and_groups()
{
}

void table_columns_and_groups::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
{
    if (CP_CHECK_NAME(L"table", L"table-column-group"))
    {
        CP_CREATE_ELEMENT_SIMPLE(content_);
    }
    else if (CP_CHECK_NAME(L"table", L"table-columns") ||
        CP_CHECK_NAME(L"table", L"table-column") ||
        CP_CHECK_NAME(L"table", L"table-header-columns"))
    {
		bool add_new_no_group = false;

		if (content_.empty())	add_new_no_group = true;
		else
		{
			if (content_.back()->get_type() != typeTableTableColumnNoGroup)
				add_new_no_group = true;
		}
		if (add_new_no_group)
		{
			_CP_PTR(table_columns_no_group) elm = table_columns_no_group::create();
			elm->add_child_element(Reader, Ns, Name, Context);
			content_.push_back(elm);
		}
		else
		{
			table_columns_no_group* cols_no_group = static_cast<table_columns_no_group*>(content_.back().get());
			cols_no_group->add_child_element(Reader, Ns, Name, Context);
		}
    }
    else
        not_applicable_element(L"table-columns-and-groups", Reader, Ns, Name);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

std::wostream & table_table_cell_content::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return CP_SERIALIZE_TEXT(elements_, bXmlEncode);
}

void table_table_cell_content::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
{
    CP_CREATE_ELEMENT_SIMPLE(elements_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-cell
const wchar_t * table_table_cell::ns = L"table";
const wchar_t * table_table_cell::name = L"table-cell";

std::wostream & table_table_cell::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return content_.text_to_stream(_Wostream, bXmlEncode);
}

void table_table_cell::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes(Attributes);
    attlist_extra_.add_attributes(Attributes);
}

void table_table_cell::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    content_.add_child_element(Reader, Ns, Name, getContext());

	if CP_CHECK_NAME(L"text", L"p")
	{
		text::p *p = dynamic_cast<text::p*>(content_.elements_.back().get());
		if (p)
		{
			is_present_hyperlink_ = p->paragraph_.is_present_hyperlink_;
		}
	}
}

void table_table_cell::add_text(const std::wstring & Text)
{
}
bool table_table_cell::empty(bool bWithStyle)
{
	if (!content_.elements_.empty()) return false;
	if (attlist_.table_formula_) return false;
	
	if (bWithStyle && attlist_.table_style_name_) return false;
	
	if (attlist_extra_.table_number_columns_spanned_ > 1) return false;
	if (attlist_extra_.table_number_rows_spanned_ > 1) return false;

	return true;
}
// table:covered-table-cell
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_covered_table_cell::ns = L"table";
const wchar_t * table_covered_table_cell::name = L"covered-table-cell";

std::wostream & table_covered_table_cell::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return content_.text_to_stream(_Wostream, bXmlEncode);
}

void table_covered_table_cell::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes(Attributes);
}

void table_covered_table_cell::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    content_.add_child_element(Reader, Ns, Name, getContext());
}

void table_covered_table_cell::add_text(const std::wstring & Text)
{
}
bool table_covered_table_cell::empty()
{
	if (!content_.elements_.empty()) return false;
	if (attlist_.table_formula_) return false;

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

// table:table-row
const wchar_t * table_table_row::ns = L"table";
const wchar_t * table_table_row::name = L"table-row";

std::wostream & table_table_row::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return CP_SERIALIZE_TEXT(content_, bXmlEncode);  
}

void table_table_row::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes( Attributes );
}

void table_table_row::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if ( CP_CHECK_NAME(L"table", L"table-cell") || 
         CP_CHECK_NAME(L"table", L"covered-table-cell") )
    {
        CP_CREATE_ELEMENT(content_);        
    }
    else
        CP_NOT_APPLICABLE_ELM();    
}
bool table_table_row::empty()
{
	if (content_.empty() && !attlist_.table_style_name_ && !attlist_.table_default_cell_style_name_) return true;

	return false;
}
bool table_table_row::empty_content_cells(bool bWithCellStyle)
{
	bool res = true;
	
	for (size_t i = 0 ; i < content_.size(); i++)
    {
		table_table_cell			*cell		= dynamic_cast<table_table_cell*>(content_[i].get());
		table_covered_table_cell	*cover_cell = dynamic_cast<table_covered_table_cell*>(content_[i].get());
		if (!cell && !cover_cell)
		{
			res = false;
			break;
		}
		if (cell && cell->empty(bWithCellStyle) == false)
		{
			res = false;
			break;
		}
		if (cover_cell && cover_cell->empty() == false)
		{
			res = false;
			break;
		}
	}
	return res;
}
// table:table-rows
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * table_table_rows::ns = L"table";
const wchar_t * table_table_rows::name = L"table-rows";

std::wostream & table_table_rows::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return CP_SERIALIZE_TEXT(table_table_row_, bXmlEncode);  
}

void table_table_rows::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void table_table_rows::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(table_table_row_);        
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-header-rows
const wchar_t * table_table_header_rows::ns = L"table";
const wchar_t * table_table_header_rows::name = L"table-header-rows";

std::wostream & table_table_header_rows::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return serialize_elements_text(_Wostream, table_table_row_, bXmlEncode);  
}

void table_table_header_rows::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void table_table_header_rows::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"table", L"table-row")
    {
        CP_CREATE_ELEMENT(table_table_row_);      

		table_table_row* row = dynamic_cast<table_table_row*>(table_table_row_.back().get());
		if (row)
			row->bHeader = true;
	}
    else
        CP_NOT_APPLICABLE_ELM();    
}

void table_table_header_rows::add_text(const std::wstring & Text)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:rows
std::wostream & table_rows::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    if (table_table_rows_)
        CP_SERIALIZE_TEXT(table_table_rows_, bXmlEncode);
    else
        CP_SERIALIZE_TEXT(table_table_row_, bXmlEncode);
    return _Wostream;
}

void table_rows::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
{
    if CP_CHECK_NAME(L"table", L"table-rows")
    {
        CP_CREATE_ELEMENT_SIMPLE(table_table_rows_);
    } 
    else if (CP_CHECK_NAME(L"table", L"table-row") || CP_CHECK_NAME(L"text", L"soft-page-break"))

    {
        CP_CREATE_ELEMENT_SIMPLE(table_table_row_);    
    }
    else
    {
        not_applicable_element(L"table-rows", Reader, Ns, Name);        
    }
}
void table_rows::remove_equals_empty()
{
	if (table_table_row_.empty()) return;

	while(true)
	{
		size_t i = table_table_row_.size() - 1;

		if (i == 0) break;

		if (table_table_row_[i]->get_type() != typeTableTableRow)
			break;
		if (table_table_row_[i-1]->get_type() != typeTableTableRow)
			break;

		table_table_row *prev = dynamic_cast<table_table_row*>(table_table_row_[i-1].get());
		table_table_row *next = dynamic_cast<table_table_row*>(table_table_row_[i].get());

		if (prev->content_.size() > 1 || next->content_.size() > 1) break;

		if (prev->attlist_.table_style_name_.get_value_or(L"") != next->attlist_.table_style_name_.get_value_or(L"")) break;

		table_table_cell *prev_cell = dynamic_cast<table_table_cell*>(prev->content_[0].get());
		table_table_cell *next_cell = dynamic_cast<table_table_cell*>(next->content_[0].get());

		if (!prev_cell || !next_cell) break;

		if (!prev_cell->content_.elements_.empty() || !next_cell->content_.elements_.empty()) break;

		if (prev_cell->attlist_.table_style_name_.get_value_or(L"") != next_cell->attlist_.table_style_name_.get_value_or(L"")) break;

		prev->attlist_.table_number_rows_repeated_ += next->attlist_.table_number_rows_repeated_;
		table_table_row_.pop_back();
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// table:rows-no-group
const wchar_t * table_rows_no_group::ns = L"table";
const wchar_t * table_rows_no_group::name = L"table-rows-no-group";

_CP_PTR(table_rows_no_group) table_rows_no_group::create()
{
    return boost::make_shared<table_rows_no_group>();
}

table_rows_no_group::table_rows_no_group() : was_header_(false)
{
};

std::wostream & table_rows_no_group::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    table_rows_1_.text_to_stream(_Wostream);
    CP_SERIALIZE_TEXT(table_table_header_rows_, bXmlEncode);
    table_rows_2_.text_to_stream(_Wostream);
    return _Wostream;    
}
void table_rows_no_group::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
}
void table_rows_no_group::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
{
    if (CP_CHECK_NAME(L"table", L"table-rows") || CP_CHECK_NAME(L"table", L"table-row") ||
		CP_CHECK_NAME(L"text", L"soft-page-break"))
    {
        if (!was_header_)
            table_rows_1_.add_child_element(Reader, Ns, Name, Context);
        else
            table_rows_2_.add_child_element(Reader, Ns, Name, Context);
    } 
    else if CP_CHECK_NAME(L"table", L"table-header-rows")
    {
        was_header_ = true;
        CP_CREATE_ELEMENT_SIMPLE(table_table_header_rows_);            
    }
    else
        not_applicable_element(L"table-rows-no-group", Reader, Ns, Name);
}
// table-rows-and-groups
//////////////////////////////////////////////////////////////////////////////////////////////////

std::wostream & table_rows_and_groups::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(content_, bXmlEncode);
    return _Wostream;
}

void table_rows_and_groups::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context)
{
    if (L"table" == Ns && L"table-row-group" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(content_);
    } 
    else if ((L"table" == Ns && (L"table-rows" == Name || L"table-row" == Name || L"table-header-rows" == Name)) ||
		(L"text" == Ns && L"soft-page-break" == Name))
    {
		bool add_new_no_group = false;
		
		if (content_.empty())	add_new_no_group = true;
		else
		{
			if (content_.back()->get_type() != typeTableTableRowNoGroup)
				add_new_no_group = true;
		}
		if (add_new_no_group)
		{
			_CP_PTR(table_rows_no_group) elm = table_rows_no_group::create();
			elm->add_child_element(Reader, Ns, Name, Context);
			content_.push_back(elm);
		}
		else
		{
			table_rows_no_group* rows_no_group = static_cast<table_rows_no_group*>(content_.back().get());
			rows_no_group->add_child_element(Reader, Ns, Name, Context);
		}
   }
    else
        not_applicable_element(L"table-rows-and-groups", Reader, Ns, Name);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:table-row-group
const wchar_t * table_table_row_group::ns = L"table";
const wchar_t * table_table_row_group::name = L"table-row-group";

std::wostream & table_table_row_group::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    return table_rows_and_groups_.text_to_stream(_Wostream, bXmlEncode);
}

void table_table_row_group::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    attlist_.add_attributes(Attributes);
}

void table_table_row_group::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    table_rows_and_groups_.add_child_element(Reader, Ns, Name, getContext());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table-table-row-group-attlist
void table_table_row_group_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"table:display", table_display_, true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:shapes
const wchar_t * table_shapes::ns = L"table";
const wchar_t * table_shapes::name = L"shapes";

void table_shapes::docx_convert(oox::docx_conversion_context & Context)
{
	for (size_t i = 0 ; i < content_.size(); i++)
	{
        content_[i]->docx_convert(Context);
    }
}
void table_shapes::pptx_convert(oox::pptx_conversion_context & Context)
{
	for (size_t i = 0 ; i < content_.size(); i++)
	{
        content_[i]->pptx_convert(Context);
    }
}

void table_shapes::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
	for (size_t i = 0 ; i < content_.size(); i++)
	{
		content_[i]->xlsx_convert(Context);
    }
}

void table_shapes::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void table_shapes::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// table:content-validations
const wchar_t * table_content_validations::ns = L"table";
const wchar_t * table_content_validations::name = L"content-validations";

void table_content_validations::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// table:content-validation
const wchar_t * table_content_validation::ns = L"table";
const wchar_t * table_content_validation::name = L"content-validation";

void table_content_validation::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
	CP_APPLY_ATTR(L"table:name",				table_name_);
    CP_APPLY_ATTR(L"table:condition",			table_condition_);
	CP_APPLY_ATTR(L"table:display-list",		table_display_list_);
	CP_APPLY_ATTR(L"table:allowempty-cell",		table_allowempty_cell_);
	CP_APPLY_ATTR(L"table:base-cell-address",	table_base_cell_address_);
}
void table_content_validation::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// table:content-validation
const wchar_t * table_error_message::ns = L"table";
const wchar_t * table_error_message::name = L"error-message";

void table_error_message::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
    CP_APPLY_ATTR(L"table:title",			table_title_);
    CP_APPLY_ATTR(L"table:message-type",	table_message_type_);
	CP_APPLY_ATTR(L"table:display",			table_display_);
}
void table_error_message::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// table:content-validation
const wchar_t * table_help_message::ns = L"table";
const wchar_t * table_help_message::name = L"help-message";

void table_help_message::add_attributes(xml::attributes_wc_ptr const & Attributes)
{
    CP_APPLY_ATTR(L"table:title",			table_title_);
	CP_APPLY_ATTR(L"table:display",			table_display_);
}
void table_help_message::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
}
}
