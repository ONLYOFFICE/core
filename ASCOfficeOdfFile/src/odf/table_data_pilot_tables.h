/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"

#include "datatypes/common_attlists.h"
#include "datatypes/bool.h"
#include "datatypes/grandtotal.h"

namespace cpdoccore { 
namespace odf_reader {

class table_data_pilot_tables : public office_element_impl<table_data_pilot_tables>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableDataPilotTables;
    CPDOCCORE_DEFINE_VISITABLE();
   
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;
  
};

CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_tables);
//-------------------------------------------------------------------------------------

class table_data_pilot_table : public office_element_impl<table_data_pilot_table>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableDataPilotTable;
    CPDOCCORE_DEFINE_VISITABLE();
   
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

     _CP_OPT(std::wstring)			table_name_;
     _CP_OPT(std::wstring)			table_application_data_;
     _CP_OPT(std::wstring)			table_buttons_;
	 _CP_OPT(odf_types::Bool)		table_drill_down_ondouble_click_;
     _CP_OPT(odf_types::grand_total)table_grand_total_;
     _CP_OPT(odf_types::Bool)		table_identify_categories_;
     _CP_OPT(odf_types::Bool)		table_ignore_empty_rows_;
     _CP_OPT(odf_types::Bool)		table_show_filterbutton_;
     _CP_OPT(odf_types::Bool)		table_show_target_range_address_;
	
	  office_element_ptr_array	content_;
};

CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_table);
//-------------------------------------------------------------------------------------

class table_data_pilot_field : public office_element_impl<table_data_pilot_field>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableDataPilotField;
    CPDOCCORE_DEFINE_VISITABLE();
   
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;  
};

CP_REGISTER_OFFICE_ELEMENT2(table_data_pilot_field);
//-------------------------------------------------------------------------------------

class table_database_source_table : public office_element_impl<table_database_source_table>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableDatabaseSourceTable;
    CPDOCCORE_DEFINE_VISITABLE();
   
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;  
};

CP_REGISTER_OFFICE_ELEMENT2(table_database_source_table);
//-------------------------------------------------------------------------------------

class table_database_source_query : public office_element_impl<table_database_source_query>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableDatabaseSourceQuery;
    CPDOCCORE_DEFINE_VISITABLE();
   
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;  
};

CP_REGISTER_OFFICE_ELEMENT2(table_database_source_query);
//-------------------------------------------------------------------------------------

class table_database_source_sql : public office_element_impl<table_database_source_sql>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableDatabaseSourceSql;
    CPDOCCORE_DEFINE_VISITABLE();
   
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;  
};

CP_REGISTER_OFFICE_ELEMENT2(table_database_source_sql);
//-------------------------------------------------------------------------------------

class table_source_cell_range : public office_element_impl<table_source_cell_range>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableSourceCellRange;
    CPDOCCORE_DEFINE_VISITABLE();
   
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;  
};

CP_REGISTER_OFFICE_ELEMENT2(table_source_cell_range);
//-------------------------------------------------------------------------------------

class table_source_service : public office_element_impl<table_source_service>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableSourceService;
    CPDOCCORE_DEFINE_VISITABLE();
   
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;  
};

CP_REGISTER_OFFICE_ELEMENT2(table_source_service);
//-------------------------------------------------------------------------------------
}
}