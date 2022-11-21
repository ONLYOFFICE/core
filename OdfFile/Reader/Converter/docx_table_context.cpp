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
#include <iostream>

#include "docx_conversion_context.h"

#include "../../Common/logging.h"
#include "../Format/odf_document.h"
#include "../Format/odfcontext.h"

namespace cpdoccore { 
namespace oox {


docx_table_state::docx_table_state(docx_conversion_context & Context, const std::wstring & StyleName) : 
	context_(Context),
    table_style_(StyleName),
    current_table_column_(-1),
    columns_spanned_num_(0),
    close_table_covered_cell_(false)
{        
}

void docx_table_state::start_column(unsigned int repeated, const std::wstring & defaultCellStyleName)
{
    columns_.push_back(repeated);

    for (unsigned int i = 0; i < repeated; ++i)
        columnsDefaultCellStyleName_.push_back(defaultCellStyleName);
}

std::wstring docx_table_state::get_default_cell_style_col(unsigned int column)
{
	if (columnsDefaultCellStyleName_.size() <= column) 
		return L"";
    return columnsDefaultCellStyleName_.at(column);
}

std::wstring docx_table_state::get_default_cell_style_row()
{
    return default_row_cell_style_name_;
}

void docx_table_state::start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName)
{
    current_table_column_ = -1;
    columns_spanned_style_ = L"";
    close_table_covered_cell_ = false;
    table_row_style_stack_.push_back(StyleName);
    default_row_cell_style_name_ = defaultCellStyleName;
}

void docx_table_state::end_row()
{
    table_row_style_stack_.pop_back();
    default_row_cell_style_name_ = L"";
}

std::wstring docx_table_state::current_row_style() const
{
    if (table_row_style_stack_.size())
        return table_row_style_stack_.back();
    else
        return L"";
}

double docx_table_state::get_current_cell_width()
{
	if (current_table_column_ + columns_spanned_num_ < columns_width_.size())
	{
		//return columns_width_[current_table_column_];
		double res = 0;
		for (unsigned int i = 0; i < columns_spanned_num_ + 1; i++)
		{
			res += columns_width_[current_table_column_ + i];
		}
		return res;
	}
	else
		return 0;
}

void docx_table_state::add_column_width(double width)
{
	columns_width_.push_back(width);
}

void docx_table_state::start_cell()
{
    current_table_column_++;
    if (current_table_column_ >= (int)(rows_spanned_.size()))
        rows_spanned_.push_back(table_row_spanned());            
}

void docx_table_state::end_cell()
{}
bool docx_table_state::start_covered_cell(docx_conversion_context & Context)
{
    std::wostream & _Wostream = context_.output_stream();
    current_table_column_++;

    // обновляем вектор, в котором хранятся информация об объединении строк
    // добавляем в него новый столбец

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

        odf_reader::style_instance * inst = 
            context_.root()->odf_context().styleContainer().style_by_name( 
					rows_spanned_[current_table_column_].style() , odf_types::style_family::TableCell,Context.process_headers_footers_);

        if (inst && inst->content())
            inst->content()->docx_convert(context_);

        _Wostream << L"</w:tcPr>";
    }

    // использовали текущую ячейку, уменьшаем счетчики оставшихся объединенных ячеек
    // для столбцов и строк

    if (columns_spanned_num_ > 0)
        columns_spanned_num_--;

    if (rows_spanned_[current_table_column_].num() > 0)
        rows_spanned_[current_table_column_].decrease();                                

    // устанавливаем флаг что ячейка была открыта, записан тег <w:tc>
    close_table_covered_cell_ = closeTag;
    return closeTag;
}

void docx_table_state::end_covered_cell()
{
    std::wostream & _Wostream = context_.output_stream();
    if (close_table_covered_cell_)
    {
        // закрываем открытую ячейку
        _Wostream << L"</w:tc>";
        close_table_covered_cell_ = false;
    }
}

int docx_table_state::current_column() const
{
    return current_table_column_;
}

void docx_table_state::set_columns_spanned(unsigned int Val)
{
    if ( current_columns_spaned() > 0 )
    {
        _CP_LOG << L"[warning] set_columns_spanned warning\n";
    }

    columns_spanned_num_ = Val;
}

unsigned int docx_table_state::current_columns_spaned() const
{
    return columns_spanned_num_;
}

void docx_table_state::set_rows_spanned(unsigned int Column, unsigned int Val, unsigned int ColumnsSpanned, const std::wstring & Style)
{
    if (rows_spanned_.size() <= Column)
    {
        _CP_LOG << L"[warning] set_rows_spanned error\n";
    }
    else
    {
        if (rows_spanned_[Column].num() > 0)
        {
            _CP_LOG << L"[warning] set_rows_spanned warning\n";
        }
        rows_spanned_[Column].num(Val);
        rows_spanned_[Column].set_style(Style);
        rows_spanned_[Column].column_spanned(ColumnsSpanned);
    }        
}

unsigned int docx_table_state::current_rows_spanned(unsigned int Column) const
{
    if (rows_spanned_.size() <= Column)
    {
        _CP_LOG << L"[warning] current_rows_spanned error\n";
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
