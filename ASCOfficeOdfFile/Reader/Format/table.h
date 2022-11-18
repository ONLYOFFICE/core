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

#include "office_elements.h"
#include "office_elements_create.h"

#include "table_named_expressions.h"
#include "table_database_ranges.h"
#include "table_data_pilot_tables.h"
#include "calcext_elements.h"

#include "../../DataTypes/tablemode.h"
#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/tablevisibility.h"
#include "../../DataTypes/messagetype.h"

namespace cpdoccore { 
namespace odf_reader {

class table_table_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

	_CP_OPT(bool)			table_is_sub_table_;
    _CP_OPT(std::wstring)	table_name_;
    _CP_OPT(std::wstring)	table_style_name_;
	_CP_OPT(std::wstring)	table_template_name_;

	bool					table_protected_;					// default false
    _CP_OPT(std::wstring)	table_protection_key_; 
	_CP_OPT(std::wstring)	table_protection_key_digest_algorithm_;

    bool					table_print_;						// default true
    _CP_OPT(std::wstring)	table_print_ranges_;

	bool					table_use_first_row_styles_;		// default false;
	bool					table_use_banding_rows_styles_;		// defualt false;
	bool					table_use_first_column_styles_;		// defualt false;
	bool					table_use_banding_columns_styles_;	// defualt false;

    friend class table_table;    
};

class table_table_row_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    unsigned int					table_number_rows_repeated_; // default 1
	_CP_OPT(std::wstring)			table_style_name_;
    _CP_OPT(std::wstring)			table_default_cell_style_name_;
    odf_types::table_visibility		table_visibility_;			// default Visible

};

class table_table_cell_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    unsigned int				table_number_columns_repeated_; // default 1
    _CP_OPT(std::wstring)		table_style_name_;
    _CP_OPT(std::wstring)		table_content_validation_name_;
    _CP_OPT(std::wstring)		table_formula_;

    odf_types::common_value_and_type_attlist	common_value_and_type_attlist_;

    bool table_protect_; // default false
};

class table_table_cell_attlist_extra
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    unsigned int			table_number_columns_spanned_; // default 1
    unsigned int			table_number_rows_spanned_; // default 1

    _CP_OPT(unsigned int)	table_number_matrix_columns_spanned_;
    _CP_OPT(unsigned int)	table_number_matrix_rows_spanned_;

};

class table_table_source_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    odf_types::table_mode table_mode_; // default CopyAll
    _CP_OPT(std::wstring) table_table_name_;
    
};

class table_linked_source_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    
    odf_types::common_xlink_attlist xlink_attlist_;

    _CP_OPT(std::wstring)			table_filter_name_;
    _CP_OPT(std::wstring)			table_filter_options_;
    _CP_OPT(std::wstring)			table_refresh_delay_;        

};

class table_table_source : public office_element_impl<table_table_source>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTableSource;

    CPDOCCORE_DEFINE_VISITABLE();

    table_table_source_attlist table_table_source_attlist_;
    table_linked_source_attlist table_linked_source_attlist_;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
};

CP_REGISTER_OFFICE_ELEMENT2(table_table_source);

class table_columns
{
public:
    void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context);
   
	void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context) ;

    office_element_ptr			table_table_columns_;
    office_element_ptr_array	table_table_column_;
    
};

class table_columns_no_group: public office_element
{
public:
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;
    const wchar_t * ns;
    const wchar_t * name;
	static const xml::NodeType xml_type = xml::typeElement;
	static const ElementType type = typeTableTableColumnNoGroup;

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ) {}
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name) {}
    virtual void add_text(const std::wstring & Text) {}
	virtual void add_space(const std::wstring & Text) {}

    CPDOCCORE_DEFINE_VISITABLE();

    table_columns_no_group();
    void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context);

    void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

    static _CP_PTR(table_columns_no_group) create();
    
    table_columns		table_columns_1_;
    
    bool was_header_;
    office_element_ptr	table_table_header_columns_;
    table_columns		table_columns_2_;    
};

class table_columns_and_groups
{
public:
    table_columns_and_groups();
    void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context);

    void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);


//    office_element_ptr			table_table_column_group_;
//    table_columns_no_group		table_columns_no_group_;
	office_element_ptr_array		content_;

};

class table_table_column_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    unsigned int				table_number_columns_repeated_; // default 1
    _CP_OPT(std::wstring)		table_style_name_;
    odf_types::table_visibility	table_visibility_;				// default Visible
    _CP_OPT(std::wstring)		table_default_cell_style_name_;
    
};

class table_table_column : public office_element_impl<table_table_column>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTableColumn;

    CPDOCCORE_DEFINE_VISITABLE();
 
	virtual void docx_convert(oox::docx_conversion_context & Context) ;
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    table_table_column_attlist	table_table_column_attlist_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_table_column);

class table_table_columns : public office_element_impl<table_table_columns>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTableColumns;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context) ;
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    office_element_ptr_array	table_table_column_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_table_columns);

class table_table_header_columns : public office_element_impl<table_table_header_columns>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTableHeaderColumns;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    office_element_ptr_array	table_table_column_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_table_header_columns);

class table_table_column_group_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    bool table_display_; // default true

};

class table_table_column_group : public office_element_impl<table_table_column_group>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTableColumnGroup;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

    table_table_column_group() 
    {
    }

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    table_table_column_group_attlist	attlist_;
    table_columns_and_groups			table_columns_and_groups_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_table_column_group);

class table_table_row : public office_element_impl<table_table_row>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTableRow;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
	bool empty();
	bool empty_content_cells(bool bWithCellStyle = true);

	bool bHeader = false;

    table_table_row_attlist		attlist_;
    office_element_ptr_array	content_; // table-table-cell or table-covered-table-cell
    
};
CP_REGISTER_OFFICE_ELEMENT2(table_table_row);

class table_table_cell_content
{
public:
    std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context);
    
    bool	docx_convert(oox::docx_conversion_context & Context) ;
    int		xlsx_convert(oox::xlsx_conversion_context & Context, text_format_properties_content_ptr text_properties) ;
    bool	pptx_convert(oox::pptx_conversion_context & Context) ;

    office_element_ptr_array	elements_; 

	// TODO table-cell-range-source
    // TODO table-detective
};

class table_table_cell : public office_element_impl<table_table_cell>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTableCell;

    CPDOCCORE_DEFINE_VISITABLE();

	table_table_cell() : last_cell_(false), is_present_hyperlink_(false) { }

    virtual void docx_convert(oox::docx_conversion_context & Context) ;
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

public:
	bool is_present_hyperlink_;
	bool last_cell_;
	bool empty(bool bWithStyle = true);

    table_table_cell_attlist		attlist_;
    table_table_cell_attlist_extra	attlist_extra_;
    table_table_cell_content		content_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_table_cell);

class table_covered_table_cell : public office_element_impl<table_covered_table_cell>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableCoveredTableCell;

    CPDOCCORE_DEFINE_VISITABLE();

	table_covered_table_cell() : last_cell_(false){}

    virtual void docx_convert(oox::docx_conversion_context & Context) ;
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);


public:
	bool last_cell_;
	bool empty();

    table_table_cell_attlist	attlist_;
    table_table_cell_content	content_;

};

CP_REGISTER_OFFICE_ELEMENT2(table_covered_table_cell);

class table_table_rows : public office_element_impl<table_table_rows>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTableRows;

    CPDOCCORE_DEFINE_VISITABLE();
  
	virtual void docx_convert(oox::docx_conversion_context & Context) ;
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;
  
	table_table_rows() 
	{
    }
    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    office_element_ptr_array	table_table_row_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_table_rows);

class table_table_header_rows : public office_element_impl<table_table_header_rows>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTableHeaderRows;

    CPDOCCORE_DEFINE_VISITABLE();
   
	virtual void docx_convert(oox::docx_conversion_context & Context) ;
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

public:
    office_element_ptr_array table_table_row_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_table_header_rows);

class table_rows
{
public:
    std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context);
  
	void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context) ;

    office_element_ptr			table_table_rows_;
    office_element_ptr_array	table_table_row_;

	void remove_equals_empty();
};

class table_rows_no_group : public office_element
{
public:
    CPDOCCORE_OFFICE_DOCUMENT_IMPL_NAME_FUNCS_;
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTableRowNoGroup;

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ) {}
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
    virtual void add_text(const std::wstring & Text) {}
    virtual void add_space(const std::wstring & Text) {}

    static _CP_PTR(table_rows_no_group) create();

    CPDOCCORE_DEFINE_VISITABLE();

    table_rows_no_group();

    std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context);
  
	void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context) ;
   
    bool was_header_;

    table_rows			table_rows_1_;    
    office_element_ptr	table_table_header_rows_;
    table_rows			table_rows_2_;
    
};

class table_rows_and_groups
{
public:
	table_rows_and_groups() {}
    std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;
    
	void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context);
  
	void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

    office_element_ptr_array	content_;
};

class table_table_row_group_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    bool table_display_; // default true
    
};
//--------------------------------------------------------------------------------------------
class table_table_row_group : public office_element_impl<table_table_row_group>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTableRowGroup;

    CPDOCCORE_DEFINE_VISITABLE();
    
    table_table_row_group()
	{
    }
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    virtual std::wostream & text_to_stream(std::wostream & _Wostream, bool bXmlEncode = true) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    table_table_row_group_attlist	attlist_;
    table_rows_and_groups			table_rows_and_groups_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_table_row_group);
//--------------------------------------------------------------------------------------------
class table_table_protection : public office_element_impl<table_table_protection>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTableProtection;

    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context) {}
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context) {}
	virtual void pptx_convert(oox::pptx_conversion_context & Context) {}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name){}

public:
	_CP_OPT(bool)	select_protected_cells;
	_CP_OPT(bool)	select_unprotected_cells;
	_CP_OPT(bool)	insert_columns;
	_CP_OPT(bool)	insert_rows;

	_CP_OPT(bool)	delete_columns;
	_CP_OPT(bool)	delete_rows;
	//_CP_OPT(bool)	format_columns; //???
	//_CP_OPT(bool)	format_rows;
	//_CP_OPT(bool)	format_cells;
};
CP_REGISTER_OFFICE_ELEMENT2(table_table_protection);
//--------------------------------------------------------------------------------------------
class table_table : public office_element_impl<table_table>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTable;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context) ;
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    table_table_attlist			attlist_;
	
	office_element_ptr			table_protection_;

    table_columns_and_groups	table_columns_and_groups_;
    table_rows_and_groups		table_rows_and_groups_;

    office_element_ptr			table_table_source_;	

	office_element_ptr			conditional_formats_;

    office_element_ptr			table_shapes_;
	office_element_ptr_array	table_named_;
	office_element_ptr			office_forms_;
    //office-dde-source
    //table-scenario
};
CP_REGISTER_OFFICE_ELEMENT2(table_table);
//--------------------------------------------------------------------------------------------
class table_sub_table : public table_table
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;
	static const xml::NodeType xml_type = xml::typeElement;
	static const ElementType type = typeTableSubTable;
};
CP_REGISTER_OFFICE_ELEMENT2(table_sub_table);
//--------------------------------------------------------------------------------------------
//table:content-shapes
class table_shapes : public office_element_impl<table_shapes>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableShapes;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context) ;
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_shapes);

//table:content-validations
class table_content_validations : public office_element_impl<table_content_validations>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableContentValidations;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ){}
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    office_element_ptr_array content_;

};
CP_REGISTER_OFFICE_ELEMENT2(table_content_validations);
//--------------------------------------------------------------------------------------------
//table:content-validation
class table_content_validation : public office_element_impl<table_content_validation>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableContentValidation;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;

private:
	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

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
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableErrorMassage;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    _CP_OPT(std::wstring) table_title_;
	_CP_OPT(odf_types::Bool) table_display_;
    _CP_OPT(odf_types::message_type) table_message_type_;

    office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_error_message);
//--------------------------------------------------------------------------------------------
//table:help-message
class table_help_message : public office_element_impl<table_help_message>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableHelpMassage;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;

	virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

	_CP_OPT(std::wstring) table_title_;
	_CP_OPT(odf_types::Bool) table_display_;
	
	office_element_ptr_array content_;
};
CP_REGISTER_OFFICE_ELEMENT2(table_help_message);

//table:error-macro
}
}
