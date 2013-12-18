#include "precompiled_cpodf.h"
#include "pptx_conversion_context.h"
#include "logging.h"
#include <boost/foreach.hpp>
#include <iostream>
#include <cpdoccore/odf/odf_document.h>
#include "../odf/odfcontext.h"

namespace cpdoccore { 
namespace oox {

/**/

pptx_table_state::pptx_table_state(pptx_conversion_context & Context,
    const std::wstring & StyleName) : context_(Context),
    table_style_(StyleName),
    current_table_column_(-1),
    columns_spanned_num_(0),
    close_table_covered_cell_(false)
{        
}

void pptx_table_state::start_column(unsigned int repeated, const std::wstring & defaultCellStyleName)
{
    columns_.push_back(repeated);

    for (unsigned int i = 0; i < repeated; ++i)
        columnsDefaultCellStyleName_.push_back(defaultCellStyleName);
}

std::wstring pptx_table_state::get_default_cell_style_col(unsigned int column)
{
    return columnsDefaultCellStyleName_.at(column);
}

std::wstring pptx_table_state::get_default_cell_style_row()
{
    return default_row_cell_style_name_;
}

void pptx_table_state::start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName)
{
    current_table_column_ = -1;
    columns_spanned_style_ = L"";
    close_table_covered_cell_ = false;
    table_row_style_stack_.push_back(StyleName);
    default_row_cell_style_name_ = defaultCellStyleName;
}

void pptx_table_state::end_row()
{
    table_row_style_stack_.pop_back();
    default_row_cell_style_name_ = L"";
}

std::wstring pptx_table_state::current_row_style() const
{
    if (table_row_style_stack_.size())
        return table_row_style_stack_.back();
    else
        return L"";
}

void pptx_table_state::start_cell()
{
    current_table_column_++;
    if (current_table_column_ >= (int)(rows_spanned_.size()))
        rows_spanned_.push_back(table_row_spanned());            
}

void pptx_table_state::end_cell()
{}

bool pptx_table_state::start_covered_cell(pptx_conversion_context & Context)
{
    std::wostream & _Wostream = context_.get_table_context().tableData();
    current_table_column_++;

    // обновл€ем вектор, в котором хран€тс€ информаци€ об объединении строк
    // добавл€ем в него новый столбец

    if (current_table_column_ >= (int)(rows_spanned_.size()))
        rows_spanned_.push_back(table_row_spanned());

    bool closeTag = false;
    if (columns_spanned_num_ == 0 && rows_spanned_[current_table_column_].num() > 0)
    {
        closeTag = true;
        _Wostream << L"<w:tc>";
        _Wostream << L"<w:tcPr>";
        _Wostream << L"<w:vMerge w:val=\"continue\" />";
		//_Wostream << L"<w:tcW w:w=\"0\" w:type=\"auto\" />";

        if (rows_spanned_[current_table_column_].column_spanned() > 0)
            _Wostream << L"<w:gridSpan w:val=\"" << rows_spanned_[current_table_column_].column_spanned() + 1 << "\" />";

        odf::style_instance * inst = 
            context_.root()->odf_context().styleContainer().style_by_name( 
					rows_spanned_[current_table_column_].style() , odf::style_family::TableCell,false);

        if (inst && inst->content())
            inst->content()->pptx_convert(context_);

        _Wostream << L"</w:tcPr>";
    }

    // использовали текущую €чейку, уменьшаем счетчики оставшихс€ объединенных €чеек
    // дл€ столбцов и строк

    if (columns_spanned_num_ > 0)
        columns_spanned_num_--;

    if (rows_spanned_[current_table_column_].num() > 0)
        rows_spanned_[current_table_column_].decrease();                                

    // устанавливаем флаг что €чейка была открыта, записан тег <w:tc>
    close_table_covered_cell_ = closeTag;
    return closeTag;
}

void pptx_table_state::end_covered_cell()
{
    std::wostream & _Wostream = context_.get_table_context().tableData();
    if (close_table_covered_cell_)
    {
        // закрываем открытую €чейку
        _Wostream << L"</w:tc>";
        close_table_covered_cell_ = false;
    }
}

int pptx_table_state::current_column() const
{
    return current_table_column_;
}

void pptx_table_state::set_columns_spanned(unsigned int Val)
{
    if ( current_columns_spaned() > 0 )
    {
 #ifdef _DEBUG
		_CP_LOG(info) << L"[warning] set_columns_spanned warning\n";
 #endif
    }

    columns_spanned_num_ = Val;
}

unsigned int pptx_table_state::current_columns_spaned() const
{
    return columns_spanned_num_;
}

void pptx_table_state::set_rows_spanned(unsigned int Column, unsigned int Val, unsigned int ColumnsSpanned, const std::wstring & Style)
{
    if (rows_spanned_.size() <= Column)
    {
        _CP_LOG(info) << L"[warning] set_rows_spanned error\n";
    }
    else
    {
        if (rows_spanned_[Column].num() > 0)
        {
            _CP_LOG(info) << L"[warning] set_rows_spanned warning\n";        
        }
        rows_spanned_[Column].num(Val);
        rows_spanned_[Column].set_style(Style);
        rows_spanned_[Column].column_spanned(ColumnsSpanned);
    }        
}

unsigned int pptx_table_state::current_rows_spanned(unsigned int Column) const
{
    if (rows_spanned_.size() <= Column)
    {
        _CP_LOG(info) << L"[warning] current_rows_spanned error\n";        
        return 0;
    }
    else
    {
        return rows_spanned_[Column].num();
    }
}

/**/

}
}
