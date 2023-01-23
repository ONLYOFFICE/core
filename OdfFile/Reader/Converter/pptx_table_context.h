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
    
	std::wstring default_cell_style_name_;

private:
	pptx_conversion_context & context_;    
   
	std::wstring table_style_;

    std::vector<std::wstring> table_row_style_stack_;
    
	std::wstring default_row_cell_style_name_;
    int current_table_column_;
   
	unsigned int columns_spanned_num_;
    std::wstring columns_spanned_style_;
    
	std::vector<table_row_spanned> rows_spanned_;
	
	bool close_table_covered_cell_;
   
	std::vector<unsigned int> columns_;
    std::vector<std::wstring> columnsDefaultCellStyleName_;
   
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
	std::wstring get_default_cell_style()
	{
		return table_states_.back().default_cell_style_name_;
	}
private:
	std::wstringstream			output_stream_;
    pptx_conversion_context &	context_;
    std::vector<pptx_table_state> table_states_;
};

void oox_serialize_tcPr(std::wostream & strm, std::vector<const odf_reader::style_instance *> & style_inst, oox::pptx_conversion_context & Context);

}
}
