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
#pragma once

#include <iosfwd>
#include <CPOptional.h>
#include <xml/xmlelement.h>
#include <xml/nodetype.h>

#include "office_elements_create.h"

#include "table_named_expressions.h"
#include "table_database_ranges.h"

#include "../../DataTypes/tablevisibility.h"
#include "../../DataTypes/messagetype.h"
#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/tablemode.h"

namespace cpdoccore { 
namespace odf_writer {

// table-table-attlist
class table_table_attlist
{
public:
	table_table_attlist()
	{
		table_use_first_row_styles_			= false;
		table_use_banding_rows_styles_		= false;
		table_use_first_column_styles_		= false;
		table_use_banding_columns_styles_	= false;
	}
    unsigned int table_number_columns_repeated_; // default 1

    _CP_OPT(std::wstring)		table_name_;
    _CP_OPT(std::wstring)		table_style_name_;
	_CP_OPT(std::wstring)		table_template_name_;

    _CP_OPT(std::wstring)		table_print_ranges_;

	_CP_OPT(odf_types::Bool)	table_protected_; // default false
    _CP_OPT(std::wstring)		table_protection_key_; 
	_CP_OPT(std::wstring)		table_protection_key_digest_algorithm_;
    
	bool table_use_first_row_styles_;		// default false;
	bool table_use_banding_rows_styles_;	// defualt false;
	bool table_use_first_column_styles_;	// defualt false;
	bool table_use_banding_columns_styles_; // defualt false;

 	void serialize(CP_ATTR_NODE);    

	friend class table_table;    
};

//-----------------------------------------------------------------------------------------------------
class table_table_row_attlist
{
public:
	table_table_row_attlist() : table_number_rows_repeated_(1){}

	unsigned int table_number_rows_repeated_; // default 1
    
	_CP_OPT(std::wstring)			table_style_name_;
    _CP_OPT(std::wstring)			table_default_cell_style_name_;
    _CP_OPT(odf_types::table_visibility)	table_visibility_; // default Visible

	void serialize(CP_ATTR_NODE);    
};

//-----------------------------------------------------------------------------------------------------
class table_table_cell_attlist
{
public:
	table_table_cell_attlist() : table_number_columns_repeated_(1) {}

	unsigned int				table_number_columns_repeated_; // default 1
    _CP_OPT(std::wstring)		table_style_name_;
    _CP_OPT(std::wstring)		table_content_validation_name_;
    _CP_OPT(std::wstring)		table_formula_;

    _CP_OPT(odf_types::common_value_and_type_attlist) common_value_and_type_attlist_;

    _CP_OPT(odf_types::Bool)			table_protect_; // default false
        
	void serialize(CP_ATTR_NODE);    
};

//-----------------------------------------------------------------------------------------------------
class table_table_cell_attlist_extra
{
public:
    _CP_OPT(unsigned int) table_number_columns_spanned_;	// default 1 
    _CP_OPT(unsigned int) table_number_rows_spanned_;		// default 1
    _CP_OPT(unsigned int) table_number_matrix_columns_spanned_;
    _CP_OPT(unsigned int) table_number_matrix_rows_spanned_;
	
	void serialize(CP_ATTR_NODE);   
};

// table-table-source-attlist
class table_table_source_attlist
{
public:
	table_table_source_attlist() : table_mode_(odf_types::table_mode::CopyAll) {}
   
	odf_types::table_mode table_mode_; // default CopyAll
    _CP_OPT(std::wstring) table_table_name_;
 
	void serialize(CP_ATTR_NODE);   
};

// table-linked-source-attlist
class table_linked_source_attlist
{
public:
    odf_types::common_xlink_attlist common_xlink_attlist_;

    _CP_OPT(std::wstring) table_filter_name_;
    _CP_OPT(std::wstring) table_filter_options_;
    _CP_OPT(std::wstring) table_refresh_delay_;        

	void serialize(CP_ATTR_NODE);    
};
//-----------------------------------------------------------------------------------------------------
class table_table_protection : public office_element_impl<table_table_protection>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTableProtection;

    

	table_table_protection() : select_protected_cells(true), select_unprotected_cells(true) {}

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	odf_types::Bool	select_protected_cells;
	odf_types::Bool	select_unprotected_cells;

	_CP_OPT(odf_types::Bool)	insert_columns;
	_CP_OPT(odf_types::Bool)	insert_rows;

	_CP_OPT(odf_types::Bool)	delete_columns;
	_CP_OPT(odf_types::Bool)	delete_rows;
	//_CP_OPT(bool)	format_columns; //???
	//_CP_OPT(bool)	format_rows;
	//_CP_OPT(bool)	format_cells;
};
CP_REGISTER_OFFICE_ELEMENT2(table_table_protection);
//-----------------------------------------------------------------------------------------------------
class table_table_source : public office_element_impl<table_table_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTableSource;

    

    table_table_source_attlist table_table_source_attlist_;
    table_linked_source_attlist table_linked_source_attlist_;

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

};

CP_REGISTER_OFFICE_ELEMENT2(table_table_source);

//-----------------------------------------------------------------------------------------------------
class table_columns
{
public:
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

public:
    office_element_ptr				table_table_columns_;
    office_element_ptr_array		content_;
    
};

//-----------------------------------------------------------------------------------------------------
class table_columns_no_group : public office_element_impl<table_columns_no_group>
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const ElementType type = typeTableTableColumnNoGroup;

	table_columns_no_group(odf_conversion_context * _Context);

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name/*, odf_conversion_context * Context*/);
	virtual void add_child_element( const office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

    static _CP_PTR(table_columns_no_group) create(odf_conversion_context * Context);

    table_columns table_columns_1_;
    
    bool was_header_;
    office_element_ptr table_table_header_columns_;

    table_columns table_columns_2_;

private:
	odf_conversion_context * Context;    
};

//-----------------------------------------------------------------------------------------------------
class table_columns_and_groups
{
public:
    table_columns_and_groups();

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context);
    virtual void add_child_element( const office_element_ptr & child_element, odf_conversion_context * Context);

    virtual void serialize(std::wostream & _Wostream);
public:

//    office_element_ptr table_table_column_group_;
//    table_columns_no_group table_columns_no_group_;
    office_element_ptr_array content_;

};

//-----------------------------------------------------------------------------------------------------
class table_table_column_attlist
{
public:
	table_table_column_attlist() :	table_number_columns_repeated_(1){}

    unsigned int							table_number_columns_repeated_; // default 1    
	_CP_OPT(std::wstring)					table_style_name_;
    _CP_OPT(odf_types::table_visibility)	table_visibility_;  // default Visible
    _CP_OPT(std::wstring)					table_default_cell_style_name_;

	void serialize(CP_ATTR_NODE);    
};

//-----------------------------------------------------------------------------------------------------
class table_table_column : public office_element_impl<table_table_column>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTableColumn;

    
 
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

public:
    table_table_column_attlist attlist_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_table_column);

//-----------------------------------------------------------------------------------------------------
class table_table_columns : public office_element_impl<table_table_columns>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTableColumns;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_table_columns);

//-----------------------------------------------------------------------------------------------------
class table_table_header_columns : public office_element_impl<table_table_header_columns>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTableHeaderColumns;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_table_header_columns);

// table-table-column-group-attlist
class table_table_column_group_attlist
{
public:
	table_table_column_group_attlist() : table_display_(true) {}
	bool table_display_; // default true

};

// \class   table_table_column_group
//          table:table-column-group
//          table-table-column-group
class table_table_column_group : public office_element_impl<table_table_column_group>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTableColumnGroup;    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    table_table_column_group() 
    {
    }

    table_table_column_group_attlist	table_table_column_group_attlist_;
    table_columns_and_groups			table_columns_and_groups_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_table_column_group);

//-----------------------------------------------------------------------------------------------------
class table_table_row : public office_element_impl<table_table_row>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTableRow;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    table_table_row_attlist		attlist_;
    office_element_ptr_array	content_; // table-table-cell, table-covered-table-cell
    
};

CP_REGISTER_OFFICE_ELEMENT2(table_table_row);
//-----------------------------------------------------------------------------------------------------
class table_table_cell : public office_element_impl<table_table_cell>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTableCell;

	table_table_cell() { }

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    table_table_cell_attlist		attlist_;
    table_table_cell_attlist_extra	attlist_extra_;

	office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_table_cell);

//-----------------------------------------------------------------------------------------------------
class table_covered_table_cell : public office_element_impl<table_covered_table_cell>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableCoveredTableCell;

    table_covered_table_cell() {empty_ = true; }

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	bool empty_;
    table_table_cell_attlist attlist_;
	office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_covered_table_cell);

//-----------------------------------------------------------------------------------------------------
class table_table_rows : public office_element_impl<table_table_rows>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTableRows;

    
  
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
  
	table_table_rows() 
	{
    }

    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_table_rows);

//-----------------------------------------------------------------------------------------------------
class table_table_header_rows : public office_element_impl<table_table_header_rows>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTableHeaderRows;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_table_header_rows);
//-----------------------------------------------------------------------------------------------------
class table_rows
{
public:
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr			table_table_rows_;
    office_element_ptr_array	content_;

};

//-----------------------------------------------------------------------------------------------------
class table_rows_no_group : public office_element_impl<table_rows_no_group>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTableRowNoGroup;

    static _CP_PTR(table_rows_no_group) create(odf_conversion_context * Context);

    table_rows_no_group(odf_conversion_context * Context);

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
   

    table_rows table_rows_1_;
    
    bool was_header_;
    office_element_ptr table_table_header_rows_;

    table_rows table_rows_2_;
private:
	odf_conversion_context * Context;
    
};

//-----------------------------------------------------------------------------------------------------
class table_rows_and_groups
{
public:
    table_rows_and_groups();

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context);
    virtual void add_child_element( const office_element_ptr & child_element, odf_conversion_context * Context);

    virtual void serialize(std::wostream & _Wostream);

	int get_count(){return (int)content_.size();}//временно .. для группировок

    office_element_ptr_array content_;
    //int type_;
    //office_element_ptr table_table_row_group_;
    //table_rows_no_group table_rows_no_group_;

};

//-----------------------------------------------------------------------------------------------------
class table_table_row_group_attlist
{
public:
	table_table_row_group_attlist() : table_display_(true) {}
    bool table_display_; // default true
};
//-----------------------------------------------------------------------------------------------------
class table_table_row_group : public office_element_impl<table_table_row_group>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTableRowGroup;

    
    
    table_table_row_group() 
	{
    }
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    table_table_row_group_attlist table_table_row_group_attlist_;
    table_rows_and_groups table_rows_and_groups_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_table_row_group);

//-----------------------------------------------------------------------------------------------------
class table_table : public office_element_impl<table_table>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableTable;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    table_table_attlist			attlist_;

	office_element_ptr			table_table_source_;//table-table-source
	office_element_ptr			table_protection_;

	office_element_ptr			table_named_expressions_;
 	office_element_ptr			table_conditional_formats_;
	office_element_ptr			table_shapes_;
    table_columns_and_groups	table_columns_and_groups_;//table-columns-and-groups
    table_rows_and_groups		table_rows_and_groups_;

 	office_element_ptr			office_forms_;
   //office-dde-source
    //table-scenario
};

CP_REGISTER_OFFICE_ELEMENT2(table_table);

//-----------------------------------------------------------------------------------------------------
class table_shapes : public office_element_impl<table_shapes>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableShapes;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array content_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_shapes);

//table:content-validations
class table_content_validations : public office_element_impl<table_content_validations>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableContentValidations;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array content_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_content_validations);

//table:content-validation
class table_content_validation : public office_element_impl<table_content_validation>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableContentValidation;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    _CP_OPT(std::wstring)		table_name_;
	_CP_OPT(odf_types::Bool)	table_allowempty_cell_;
	_CP_OPT(std::wstring)		table_display_list_;
    _CP_OPT(std::wstring)		table_condition_;
    _CP_OPT(std::wstring)		table_base_cell_address_;

    office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_content_validation);

//table:error-message
class table_error_message : public office_element_impl<table_error_message>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableErrorMassage;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    _CP_OPT(std::wstring) table_title_;
	_CP_OPT(odf_types::Bool) table_display_;
    _CP_OPT(odf_types::message_type) table_message_type_;

    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_error_message);

//table:help-message
class table_help_message : public office_element_impl<table_help_message>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableHelpMassage;

    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    _CP_OPT(std::wstring)		table_title_;
	_CP_OPT(odf_types::Bool)	table_display_;
    _CP_OPT(std::wstring)		table_message_type_;

    office_element_ptr_array	content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_help_message);
}
}
