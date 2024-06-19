/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include <string>
#include <list>
#include <vector>
#include "ooxtablerowspanned.h"

namespace cpdoccore 
{ 
	namespace odf_reader
	{
		class style_instance;
	};

namespace oox {

class pptx_conversion_context;

class pptx_table_state
{
public:
    pptx_table_state(pptx_conversion_context & Context, const std::wstring & StyleName);

    std::wstring current_style() const { return table_style_; }
    void start_column(unsigned int repeated, const std::wstring & defaultCellStyleName);
    std::wstring get_default_cell_style_col(unsigned int column);
    std::wstring get_default_cell_style_row();
    void set_default_cell_style_row(const std::wstring& style_name);
    void set_default_cell_style_col(unsigned int column, const std::wstring style_name);
    
	void start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName);
    void end_row();
    
	std::wstring current_row_style() const;
    void start_cell();
    void end_cell();

    bool start_covered_cell(pptx_conversion_context & Context);
    void end_covered_cell();

    int current_column() const;

    void set_columns_spanned(unsigned int Val);
    unsigned int current_columns_spaned() const;

    void set_rows_spanned(unsigned int Column, unsigned int Val, unsigned int ColumnsSpanned, const std::wstring & Style);
    unsigned int current_rows_spanned(unsigned int Column) const;

	void set_rows(int rows);
	int get_rows() const;
    int get_current_row() const;
    void set_columns(int cols);
    int get_columns() const;
    void set_template_row_style_name(const std::wstring style_name);
    std::wstring get_template_row_style_name() const;

	std::wstring default_cell_style_name_;
    std::wstring template_row_style_name_;
    
    _CP_OPT(std::wstring) first_row_style_name_;
    _CP_OPT(std::wstring) last_row_style_name_;
    _CP_OPT(std::wstring) odd_rows_style_name_;
    
    _CP_OPT(std::wstring) first_column_style_name_;
    _CP_OPT(std::wstring) last_column_style_name_;
    _CP_OPT(std::wstring) odd_columns_style_name;
private:
	pptx_conversion_context & context_;    
   
	std::wstring table_style_;

    std::vector<std::wstring> table_row_style_stack_;
    
	std::wstring default_row_cell_style_name_;
    int current_table_column_;
   
	unsigned int columns_spanned_num_;
    std::wstring columns_spanned_style_;
    
	std::vector<table_row_spanned> rows_spanned_;
	
	std::vector<unsigned int> columns_;
    std::vector<std::wstring> columnsDefaultCellStyleName_;

    unsigned int rows_;
    unsigned int current_row_;
    unsigned int total_columns_;
};

class pptx_table_context : boost::noncopyable
{
public:
    pptx_table_context(pptx_conversion_context & Context) : context_(Context)
    {}

	std::wstringstream & tableData(){return output_stream_;}

    void start_table(const std::wstring & StyleName)
    {
		output_stream_.str(std::wstring(L""));
        table_states_.push_back(pptx_table_state(context_, StyleName));     
    }

    void end_table()
    {
        table_states_.pop_back();        
    }

	std::wstring current_style() const
	{
		return table_states_.back().current_style();
	}

    size_t in_table() const
    {
        return table_states_.size();
    }

    void start_column(unsigned int repeated, const std::wstring & defaultCellStyleName)
    {
        return table_states_.back().start_column(repeated, defaultCellStyleName);
    }

    void start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName)
    {
        return table_states_.back().start_row(StyleName, defaultCellStyleName);
    }

    void end_row()
    {
        return table_states_.back().end_row();
    }

    std::wstring current_row_style() const
    {
        return table_states_.back().current_row_style();
    }

    void start_cell()
    {
        return table_states_.back().start_cell();
    }

    void end_cell()
    {
        return table_states_.back().end_cell();
    }

    bool start_covered_cell(pptx_conversion_context & Context)
    {
        return table_states_.back().start_covered_cell(Context);
    }

    void end_covered_cell()
    {
        return table_states_.back().end_covered_cell();
    }

    int current_column() const
    {
        return table_states_.back().current_column();
    }

    void set_columns_spanned(unsigned int Val)
    {
        return table_states_.back().set_columns_spanned(Val);
    }

    unsigned int current_columns_spaned() const
    {
        return table_states_.back().current_columns_spaned();
    }

    void set_rows_spanned(unsigned int Column, unsigned int Val, unsigned int ColumnsSpanned, const std::wstring & Style)
    {
        return table_states_.back().set_rows_spanned(Column, Val, ColumnsSpanned, Style);
    }

    unsigned int current_rows_spanned(unsigned int Column) const
    {
        return table_states_.back().current_rows_spanned(Column);
    }

    void set_default_cell_style_col(unsigned int column, std::wstring style_name)
    {
        table_states_.back().set_default_cell_style_col(column, style_name);
    }

    std::wstring get_default_cell_style_col(unsigned int column)
    {
        return table_states_.back().get_default_cell_style_col(column);
    }

    std::wstring get_default_cell_style_row()
    {
        return table_states_.back().get_default_cell_style_row();
    }

	void set_default_cell_style(std::wstring style_name)
	{
		table_states_.back().default_cell_style_name_ = style_name;
	}

    void set_table_rows(int rows)
    {
        table_states_.back().set_rows(rows);
    }

    int get_table_rows() const
    {
        return table_states_.back().get_rows();
    }

    void set_table_columns(int cols)
    {
        return table_states_.back().set_columns(cols);
    }

    int get_table_columns() const 
    {
        return table_states_.back().get_columns();
    }

    void set_first_row_style_name(std::wstring style_name)
    {
        table_states_.back().first_row_style_name_ = style_name;
    }

    void set_last_row_style_name(std::wstring style_name)
    {
        table_states_.back().last_row_style_name_ = style_name;
    }

    void set_odd_rows_style_name(std::wstring style_name)
    {
        table_states_.back().odd_rows_style_name_ = style_name;
    }

    void set_first_column_style_name(std::wstring style_name)
    {
        table_states_.back().first_column_style_name_ = style_name;
    }

    void set_last_column_style_name(std::wstring style_name)
    {
        table_states_.back().last_column_style_name_ = style_name;
    }

    void set_odd_columns_style_name(std::wstring style_name)
    {
        table_states_.back().odd_columns_style_name = style_name;
    }

    std::wstring get_first_row_style_name()
    {
        return table_states_.back().first_row_style_name_.get_value_or(L"");
    }

    std::wstring get_last_row_style_name()
    {
        return table_states_.back().last_row_style_name_.get_value_or(L"");
    }

    std::wstring get_odd_rows_style_name()
    {
        return table_states_.back().odd_rows_style_name_.get_value_or(L"");
    }

    std::wstring get_first_column_style_name()
    {
        return table_states_.back().first_column_style_name_.get_value_or(L"");
    }

    std::wstring get_last_column_style_name()
    {
        return table_states_.back().last_column_style_name_.get_value_or(L"");
    }

    std::wstring get_odd_column_style_name()
    {
        return table_states_.back().odd_columns_style_name.get_value_or(L"");
    }

	std::wstring get_default_cell_style()
	{
		return table_states_.back().default_cell_style_name_;
	}

    void set_template_row_style_name(const std::wstring& style_name)
    {
        table_states_.back().set_template_row_style_name(style_name);
    }

    std::wstring get_template_row_style_name()
    {
        return table_states_.back().get_template_row_style_name();
    }

    void set_template_use_styles(
		                        bool use_first_row_styles,
		                        bool use_last_row_styles,
		                        bool use_banding_rows_styles,
		                        bool use_first_column_styles,
		                        bool use_last_column_styles,
                                bool use_banding_columns_styles)
    {
        template_use_styles_.use_first_row_styles       = use_first_row_styles;
        template_use_styles_.use_last_row_styles        = use_last_row_styles;
        template_use_styles_.use_banding_rows_styles    = use_banding_rows_styles;

        template_use_styles_.use_first_column_styles    = use_first_column_styles;
        template_use_styles_.use_last_column_styles     = use_last_column_styles;
        template_use_styles_.use_banding_columns_styles = use_banding_columns_styles;
    }

    bool template_is_first_row()
    {
        return table_states_.back().get_current_row() == 0 && template_use_styles_.use_first_row_styles;
    }

    bool template_is_last_row()
    {
        return
            table_states_.back().get_current_row() == table_states_.back().get_rows() - 1 &&
            template_use_styles_.use_last_row_styles;
    }

    bool template_is_odd_row()
    {
        return 
            table_states_.back().get_current_row() % 2 == 1 &&
            template_use_styles_.use_banding_rows_styles;  
    }

    bool template_is_first_column()
    {
        return table_states_.back().current_column() == 0 &&
            template_use_styles_.use_first_column_styles;
    }

    bool template_is_last_column()
    {
        return
            table_states_.back().current_column() == table_states_.back().get_columns() - 1 &&
            template_use_styles_.use_last_column_styles;
    }

    bool template_is_odd_column()
    {
        return 
			table_states_.back().current_column() % 2 == 1 &&
			template_use_styles_.use_banding_columns_styles;
    }

private:
	std::wstringstream			output_stream_;
    pptx_conversion_context &	context_;
    std::vector<pptx_table_state> table_states_;

    struct template_use_styles
    {
        bool use_first_row_styles       = false;
        bool use_last_row_styles        = false;
        bool use_banding_rows_styles    = false;

        bool use_first_column_styles    = false;
        bool use_last_column_styles     = false;
        bool use_banding_columns_styles = false;
    } template_use_styles_;
};

void oox_serialize_tcPr(std::wostream & strm, std::vector<const odf_reader::style_instance *> & style_inst, oox::pptx_conversion_context & Context);

}
}
