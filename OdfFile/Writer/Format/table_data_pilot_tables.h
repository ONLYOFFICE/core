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

#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/bool.h"
#include "../../DataTypes/grandtotal.h"
#include "../../DataTypes/membertype.h"
#include "../../DataTypes/tableorientation.h"
#include "../../DataTypes/tabletype.h"
#include "../../DataTypes/tablefunction.h"
#include "../../DataTypes/tableorder.h"
#include "../../DataTypes/dategroup.h"

namespace cpdoccore { 
namespace odf_writer {

class table_data_pilot_tables : public office_element_impl<table_data_pilot_tables>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotTables;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

   office_element_ptr_array content_;  
};

CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_tables);
//-------------------------------------------------------------------------------------

class table_data_pilot_table : public office_element_impl<table_data_pilot_table>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotTable;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

     _CP_OPT(std::wstring)			table_name_;
	 _CP_OPT(std::wstring)			table_target_range_address_;
     _CP_OPT(std::wstring)			table_application_data_;
     _CP_OPT(std::wstring)			table_buttons_;
	 _CP_OPT(odf_types::Bool)		table_drill_down_ondouble_click_;
     _CP_OPT(odf_types::grand_total)table_grand_total_;
     _CP_OPT(odf_types::Bool)		table_identify_categories_;
     _CP_OPT(odf_types::Bool)		table_ignore_empty_rows_;
     _CP_OPT(odf_types::Bool)		table_show_filter_button_;
	
	office_element_ptr			source_;
	office_element_ptr_array	fields_;
	office_element_ptr			grand_total_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_table);
//-------------------------------------------------------------------------------------

class table_data_pilot_field : public office_element_impl<table_data_pilot_field>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotField;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)					table_source_field_name_;
	_CP_OPT(odf_types::table_orientation)	table_orientation_;
	_CP_OPT(int)							table_used_hierarchy_;
	_CP_OPT(odf_types::table_function)		table_function_;
	_CP_OPT(odf_types::Bool)				loext_ignore_selected_page_;
	_CP_OPT(std::wstring)					table_selected_page_;
	_CP_OPT(odf_types::Bool)				table_is_data_layout_field_;
	_CP_OPT(std::wstring)					table_display_name_;
	
	office_element_ptr	reference_;  
	office_element_ptr	level_;  
	office_element_ptr	groups_;  
};

CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_field);
//-------------------------------------------------------------------------------------

class table_data_pilot_grand_total : public office_element_impl<table_data_pilot_grand_total>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotGrandTotal;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)					table_display_name_;
	_CP_OPT(odf_types::Bool)				table_display_;
	_CP_OPT(odf_types::table_orientation)	table_orientation_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_grand_total );
//-------------------------------------------------------------------------------------
class table_data_pilot_field_reference : public office_element_impl<table_data_pilot_field_reference>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotFieldReference;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)			table_field_name_;
	_CP_OPT(std::wstring)			table_member_name_;
	_CP_OPT(odf_types::member_type)	table_member_type_;	
	_CP_OPT(odf_types::table_type)	table_type_;		
};

CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_field_reference);
//-------------------------------------------------------------------------------------

class table_database_source_table : public office_element_impl<table_database_source_table>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDatabaseSourceTable;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)	table_database_name_;
	_CP_OPT(std::wstring)	table_table_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_database_source_table);
//-------------------------------------------------------------------------------------

class table_database_source_query : public office_element_impl<table_database_source_query>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDatabaseSourceQuery;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)	table_database_name_;
	_CP_OPT(std::wstring)	table_query_name_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_database_source_query);
//-------------------------------------------------------------------------------------

class table_database_source_sql : public office_element_impl<table_database_source_sql>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDatabaseSourceSql;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)	table_database_name_;
	_CP_OPT(odf_types::Bool)table_parse_sql_statement_;
	_CP_OPT(std::wstring)	table_sql_statement_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_database_source_sql);
//-------------------------------------------------------------------------------------

class table_source_cell_range : public office_element_impl<table_source_cell_range>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableSourceCellRange;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		table_cell_range_address_;
    office_element_ptr_array	content_;  //filters
};

CP_REGISTER_OFFICE_ELEMENT2(table_source_cell_range);
//-------------------------------------------------------------------------------------
class table_source_cell_ranges : public office_element_impl<table_source_cell_ranges>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableSourceCellRanges;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring) table_cell_ranges_address_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_source_cell_ranges);
//-------------------------------------------------------------------------------------

class table_source_service : public office_element_impl<table_source_service>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableSourceService;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		table_name_;
	_CP_OPT(std::wstring)		table_object_name_;
	_CP_OPT(std::wstring)		table_password_;
	_CP_OPT(std::wstring)		table_source_name_;
	_CP_OPT(std::wstring)		table_user_name_;
	
};
CP_REGISTER_OFFICE_ELEMENT2(table_source_service);
//-------------------------------------------------------------------------------------

class table_data_pilot_level : public office_element_impl<table_data_pilot_level>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotLevel;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(odf_types::Bool)	table_show_empty_;
	_CP_OPT(odf_types::Bool)	calcext_repeat_item_labels_;

	office_element_ptr	members_;  
	office_element_ptr	subtotals_;  
	office_element_ptr	display_info_;  
	office_element_ptr	layout_info_;  
	office_element_ptr	sort_info_;  
};
CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_level);
//-------------------------------------------------------------------------------------

class table_data_pilot_groups : public office_element_impl<table_data_pilot_groups>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotGroups;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)			table_date_end_;
	_CP_OPT(std::wstring)			table_date_start_;
	_CP_OPT(std::wstring)			table_start;			//double 18.2 or auto.
	_CP_OPT(std::wstring)			table_end_;				//double 18.2 or auto.
	_CP_OPT(odf_types::date_group)	table_grouped_by_;		
	_CP_OPT(std::wstring)			table_source_field_name_;
	_CP_OPT(double)					table_step;
	
	office_element_ptr_array	content_;  
};
CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_groups);
//-------------------------------------------------------------------------------------

class table_data_pilot_group : public office_element_impl<table_data_pilot_group>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotGroup;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		table_name_;
    office_element_ptr_array	content_;  
};
CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_group);

//-------------------------------------------------------------------------------------

class table_data_pilot_members : public office_element_impl<table_data_pilot_members>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotMembers;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

   office_element_ptr_array	content_;  
};
CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_members);

//-------------------------------------------------------------------------------------

class table_data_pilot_member : public office_element_impl<table_data_pilot_member>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotMember;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		table_name_;
	_CP_OPT(odf_types::Bool)	table_display_;
	_CP_OPT(odf_types::Bool)	table_show_details_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_member);

//-------------------------------------------------------------------------------------
class table_data_pilot_group_member : public office_element_impl<table_data_pilot_group_member>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotGroupMember;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring) table_name_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_group_member);
//-------------------------------------------------------------------------------------

class table_data_pilot_subtotals : public office_element_impl<table_data_pilot_subtotals>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotSubtotals;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    office_element_ptr_array	content_;  
};

CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_subtotals);
//-------------------------------------------------------------------------------------

class table_data_pilot_subtotal : public office_element_impl<table_data_pilot_subtotal>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataPilotSubtotal;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(odf_types::table_function)	table_function_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_subtotal);
//-------------------------------------------------------------------------------------

class table_data_pilot_layout_info : public office_element_impl<table_data_pilot_layout_info>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataLayoutInfo;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(odf_types::Bool)	table_add_empty_lines_; 
	_CP_OPT(std::wstring)		table_layout_mode_; //tabular-layout, outlinesubtotals-top or outline-subtotals-bottom.
};
CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_layout_info);
//-------------------------------------------------------------------------------------

class table_data_pilot_sort_info : public office_element_impl<table_data_pilot_sort_info>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataSortInfo;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)			table_data_field_;
	_CP_OPT(odf_types::table_order)	table_order_;
	_CP_OPT(std::wstring)			table_sort_mode_;			//data, none, manual or name.
};
CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_sort_info);
//-------------------------------------------------------------------------------------

class table_data_pilot_display_info : public office_element_impl<table_data_pilot_display_info>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTableDataDisplayInfo;
    
   
	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name){}
	virtual void add_child_element( const office_element_ptr & child_element){}

    virtual void serialize(std::wostream & _Wostream);

	_CP_OPT(std::wstring)		table_data_field_;
	_CP_OPT(std::wstring)		table_display_member_mode_;	//from-top or from-bottom.
	_CP_OPT(odf_types::Bool)	table_enabled_;
	_CP_OPT(unsigned int)		table_member_count_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_display_info);
//-------------------------------------------------------------------------------------

}
}