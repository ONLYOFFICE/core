#pragma once

#include <string>
#include <list>
#include <vector>
#include "ooxtablerowspanned.h"

namespace cpdoccore { 
namespace oox {

class docx_conversion_context;

class docx_table_state
{
public:
    docx_table_state(docx_conversion_context & Context, const std::wstring & StyleName);

    std::wstring current_style() const { return table_style_; }
    void start_column(unsigned int repeated, const std::wstring & defaultCellStyleName);
    std::wstring get_default_cell_style_col(unsigned int column);
    std::wstring get_default_cell_style_row();
    void start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName);
    void end_row();
    std::wstring current_row_style() const;
   
	void start_cell();
    void end_cell();

    bool start_covered_cell(docx_conversion_context & Context);
    void end_covered_cell();

    int current_column() const;

    void			set_columns_spanned(unsigned int Val);
    unsigned int	current_columns_spaned() const;

    void			set_rows_spanned(unsigned int Column, unsigned int Val, unsigned int ColumnsSpanned, const std::wstring & Style);
    unsigned int	current_rows_spanned(unsigned int Column) const;

private:
    docx_conversion_context	&	context_;    
    std::wstring				table_style_;
    std::list<std::wstring>		table_row_style_stack_;
    std::wstring				default_row_cell_style_name_;
    int							current_table_column_;
    unsigned int				columns_spanned_num_;
    std::wstring				columns_spanned_style_;
    std::vector<table_row_spanned> rows_spanned_;
	bool						close_table_covered_cell_;
    std::vector<unsigned int>	columns_;
    std::vector<std::wstring>	columnsDefaultCellStyleName_;
    
};

class docx_table_context : boost::noncopyable
{
public:
    docx_table_context(docx_conversion_context & Context) : context_(Context)
    {}

    void start_table(const std::wstring & StyleName)
    {
        table_state_stack_.push_back(docx_table_state(context_, StyleName));     
    }

    void end_table()
    {
        table_state_stack_.pop_back();        
    }

    std::wstring current_style() const
    {
        return table_state_stack_.back().current_style();    
    }

    size_t in_table() const
    {
        return table_state_stack_.size();
    }

    void start_column(unsigned int repeated, const std::wstring & defaultCellStyleName)
    {
        return table_state_stack_.back().start_column(repeated, defaultCellStyleName);
    }

    void start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName)
    {
        return table_state_stack_.back().start_row(StyleName, defaultCellStyleName);
    }

    void end_row()
    {
        return table_state_stack_.back().end_row();
    }

    std::wstring current_row_style() const
    {
        return table_state_stack_.back().current_row_style();
    }

    void start_cell()
    {
        return table_state_stack_.back().start_cell();
    }

    void end_cell()
    {
        return table_state_stack_.back().end_cell();
    }

    bool start_covered_cell(docx_conversion_context & Context)
    {
        return table_state_stack_.back().start_covered_cell(Context);
    }

    void end_covered_cell()
    {
        return table_state_stack_.back().end_covered_cell();
    }

    int current_column() const
    {
        return table_state_stack_.back().current_column();
    }

    void set_columns_spanned(unsigned int Val)
    {
        return table_state_stack_.back().set_columns_spanned(Val);
    }

    unsigned int current_columns_spaned() const
    {
        return table_state_stack_.back().current_columns_spaned();
    }

    void set_rows_spanned(unsigned int Column, unsigned int Val, unsigned int ColumnsSpanned, const std::wstring & Style)
    {
        return table_state_stack_.back().set_rows_spanned(Column, Val, ColumnsSpanned, Style);
    }

    unsigned int current_rows_spanned(unsigned int Column) const
    {
        return table_state_stack_.back().current_rows_spanned(Column);
    }

    std::wstring get_default_cell_style_col(unsigned int column)
    {
        return table_state_stack_.back().get_default_cell_style_col(column);
    }

    std::wstring get_default_cell_style_row()
    {
        return table_state_stack_.back().get_default_cell_style_row();
    }

private:
    docx_conversion_context & context_;
    std::list<docx_table_state> table_state_stack_;
};


}
}
