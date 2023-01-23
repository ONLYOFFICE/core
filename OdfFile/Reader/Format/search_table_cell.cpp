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

#include "search_table_cell.h"
#include "office_body.h"
#include "office_spreadsheet.h"
#include "../../Reader/Converter/xlsx_row_spanned.h"
#include "../../Reader/Converter/xlsx_utils.h"
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <CPString.h>

#include "visitor.h"
#include "table.h"
#include "table_calculation_settings.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

class office_element;
typedef shared_ptr<const office_element>::Type office_element_ptr_const;

#define ACCEPT_ALL_CONTENT_CONST(VAL, STOP) \
        BOOST_FOREACH(const office_element_ptr_const & elm, (VAL)) \
        { \
            if (STOP) \
                break; \
            if (elm) \
                elm->accept(*this); \
        }
#define ACCEPT_ALL_CONTENT(VAL, STOP) \
        BOOST_FOREACH(const office_element_ptr & elm, (VAL)) \
        { \
            if (STOP) \
                break; \
            if (elm) \
                elm->accept(*this); \
        }
// обход таблицы
class table_round : public base_visitor, 
    public const_visitor<office_body>,
    public const_visitor<office_spreadsheet>,
    public const_visitor<table_table>,
    
    public const_visitor<table_table_row_group>,
    public const_visitor<table_rows_no_group>,
    public const_visitor<table_table_header_rows>,
    public const_visitor<table_table_rows>,
    public const_visitor<table_table_row>,
    public const_visitor<table_rows>,
    
    public const_visitor<table_table_cell>,
    public const_visitor<table_covered_table_cell>,    

    public const_visitor<table_table_column_group>,
    public const_visitor<table_table_header_columns>,
    public const_visitor<table_table_columns>,
    public const_visitor<table_table_column>,
    public const_visitor<table_columns_no_group>,

	public const_visitor<table_calculation_settings>
{
public:
    typedef boost::function< void (const office_element * elm, std::wstring defaultColumnStyle, std::wstring defaultRowStyle) > on_found_callback;

    table_round(std::wstring const & targetTable, int targetColumn, int targetRow, on_found_callback on_found)
        : target_table_			(targetTable),
        target_column_			(targetColumn),
        target_row_				(targetRow),
        on_found_				(on_found),
        current_table_column_	(0),
        current_table_row_		(0),
        columns_spanned_num_	(0),
        columns_count_			(0),
        stop_(false)
      {}

private:
    virtual void on_not_impl(std::string const & message)
    {
        _CP_LOG << L"[table_round visitor] : not impliment for \"" << utf8_to_utf16(message) << L"\"" << std::endl;
    }

public:    
    virtual void visit(const table_columns_no_group& val)
    {
        if (val.table_columns_1_.table_table_columns_)
            val.table_columns_1_.table_table_columns_->accept(*this);

        ACCEPT_ALL_CONTENT_CONST(val.table_columns_1_.table_table_column_, stop_);

        if (val.table_table_header_columns_)
            val.table_table_header_columns_->accept(*this);

        if (val.table_columns_2_.table_table_columns_)
            val.table_columns_2_.table_table_columns_->accept(*this);

        ACCEPT_ALL_CONTENT_CONST(val.table_columns_2_.table_table_column_, stop_);
    }

    virtual void visit(const table_table_column& val)
    {
        const unsigned int columnsRepeated = val.table_table_column_attlist_.table_number_columns_repeated_;
        const std::wstring defaultCellStyleName = val.table_table_column_attlist_.table_default_cell_style_name_.get_value_or(L"");
        visit_column(columnsRepeated, defaultCellStyleName);
    }
    virtual void visit(const table_calculation_settings & val)
    {

	}

    virtual void visit(const table_table_columns& val)
    {
        ACCEPT_ALL_CONTENT_CONST(val.table_table_column_, stop_);
    }

    virtual void visit(const table_table_header_columns& val)
    {
        ACCEPT_ALL_CONTENT_CONST(val.table_table_column_, stop_);
    }

    virtual void visit(const table_table_column_group& val)
    {
        ACCEPT_ALL_CONTENT_CONST(val.table_columns_and_groups_.content_, stop_);
    }

    virtual void visit(const office_body& val)
    {
        if (val.content_)
            val.content_->accept(*this);
    }

    virtual void visit(const office_spreadsheet& val)
    {
        ACCEPT_ALL_CONTENT_CONST(val.content_, stop_);
    }

    virtual void visit(const table_table& val)
    {
        if (visit_table(val.attlist_.table_name_.get_value_or(L"")))
        {
            ACCEPT_ALL_CONTENT_CONST(val.table_columns_and_groups_.content_, stop_);
            ACCEPT_ALL_CONTENT_CONST(val.table_rows_and_groups_.content_, stop_);
        }
    }

    virtual void visit(const table_table_row_group& val)
    {
        ACCEPT_ALL_CONTENT_CONST(val.table_rows_and_groups_.content_, stop_);
    }

    virtual void visit(const table_rows_no_group& val)
    {
        ACCEPT_ALL_CONTENT_CONST(val.table_rows_1_.table_table_row_, stop_);
        if (val.table_table_header_rows_)
            val.table_table_header_rows_->accept(*this);
        ACCEPT_ALL_CONTENT_CONST(val.table_rows_2_.table_table_row_, stop_);
    }

    virtual void visit(const table_table_header_rows& val)
    {        
        ACCEPT_ALL_CONTENT_CONST(val.table_table_row_, stop_);
    }

    virtual void visit(const table_table_rows& val)
    {        
        ACCEPT_ALL_CONTENT_CONST(val.table_table_row_, stop_);
    }

    virtual void visit(const table_table_row& val)
    {       
        unsigned int repeated = val.attlist_.table_number_rows_repeated_;
        std::wstring defaultCellStyle = val.attlist_.table_default_cell_style_name_.get_value_or(L"");

        if (visit_rows(repeated, defaultCellStyle))
        {
            ACCEPT_ALL_CONTENT_CONST(val.content_, stop_);
        }
    }

    virtual void visit(const table_table_cell& val)
    {
        unsigned int repeated = val.attlist_.table_number_columns_repeated_;
        std::wstring defaultColumnStyle, defaultRowStyle;
        if (visit_cells(repeated, defaultColumnStyle, defaultRowStyle))
        {
            stop_ = true;
            if (on_found_)
                on_found_(&val, defaultColumnStyle, defaultRowStyle);                       
        }
    }

    virtual void visit(const table_covered_table_cell& val)
    {
        unsigned int repeated = val.attlist_.table_number_columns_repeated_;
        std::wstring defaultColumnStyle, defaultRowStyle;
    
		if ( repeated <2) 
			return;

		if (visit_cells_covered(repeated, defaultColumnStyle, defaultRowStyle))
        {
            stop_ = true;
            if (on_found_)
                on_found_(&val, defaultColumnStyle, defaultRowStyle);
        }
    }

    virtual void visit(const table_rows& val)
    {
        if (val.table_table_rows_)
            val.table_table_rows_->accept(*this);
        else
        {
            ACCEPT_ALL_CONTENT_CONST(val.table_table_row_, stop_);
        }  
    }

private:
    bool visit_table(std::wstring const & name)
    {
        table_name_ = name;
        return table_name_ == target_table_;
    }

    void visit_column(unsigned int repeated, std::wstring const & defaultCellStyleName)
    {
        for (unsigned int i = 0; i < repeated; ++i)
            column_default_cell_style_name_.push_back(defaultCellStyleName);

        columns_count_ += repeated;
        columns_.push_back(repeated);    
    }

    bool visit_rows(unsigned int repeated, std::wstring const & defaultCellStyleName)
    {
        bool result = 
            target_row_ >= current_table_row_ && 
            target_row_ < current_table_row_ + repeated;

        current_table_column_ = 0;
        current_table_row_ += repeated;
        row_default_cell_style_name_ = defaultCellStyleName;
        return result;
    }

    bool visit_cells(unsigned int colRepeated,
        std::wstring & defaultColumnStyle,
        std::wstring & defaultRowStyle)
    {
        bool result = 
            target_column_ >= current_table_column_ &&
            target_column_ < current_table_column_ + colRepeated;
        current_table_column_ += colRepeated;

        if (result)
        {
            defaultColumnStyle = column_default_cell_style_name_[target_column_];
            defaultRowStyle = row_default_cell_style_name_;
        }
        return result;
    }

    bool visit_cells_covered(unsigned int colRepeated,
        std::wstring & defaultColumnStyle,
        std::wstring & defaultRowStyle)
    {
        return visit_cells(colRepeated, defaultColumnStyle, defaultRowStyle);
    }

private:
    std::wstring target_table_;
    int target_column_;
    int target_row_;
    on_found_callback on_found_;

    std::wstring table_name_;
    std::vector<std::wstring> column_default_cell_style_name_;
    int current_table_column_;
    int current_table_row_;
    unsigned int columns_spanned_num_;
    std::wstring columns_spanned_style_;
    std::vector<oox::xlsx_row_spanned> rows_spanned_;
    std::vector<unsigned int> columns_;
    std::wstring row_default_cell_style_name_;
    unsigned int columns_count_;
    bool stop_;

};

namespace 
{

class search_result_helper
{
public:
    search_result_helper() : found_(false), elm_(NULL)
    {
    }

    void operator ()(const office_element * elm, std::wstring defaultColumnStyle, std::wstring defaultRowStyle)
    {        
        elm_ = elm;
        defaultColumnStyle_ = defaultColumnStyle;
        defaultRowStyle_ = defaultRowStyle;
    }

    const office_element * element() { return elm_; }
    std::wstring defaultColumnStyle() { return defaultColumnStyle_; }
    std::wstring defaultRowStyle() { return defaultRowStyle_; }

private:
    bool found_;
    const office_element * elm_;
    std::wstring defaultColumnStyle_, defaultRowStyle_;
};

}

const office_element * search_table_cell(const office_element * base,
                                   std::wstring const & tableName,
                                   std::wstring const & ref,
                                   std::wstring & defaultColumnStyle, std::wstring & defaultRowStyle)
{
    try 
    {
        size_t c, r;
        oox::getCellAddressInv(ref, c, r);
        search_result_helper searchResult;
        table_round sv(tableName, c, r, boost::ref(searchResult));
        base->accept(sv);

        defaultColumnStyle = searchResult.defaultColumnStyle();
        defaultRowStyle = searchResult.defaultRowStyle();
        return searchResult.element();
    }
    catch(...)
    {
        return NULL;
    }
}

}
}

