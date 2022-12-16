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


#include "logging.h"

#include "odf_table_styles_context.h"
#include "odf_conversion_context.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"

namespace cpdoccore {
namespace odf_writer {

style_table_cell_properties *odf_table_styles_context::get_table_cell_properties()
{
	if (current == NULL)return NULL;

	if (!current->table_cell_props)
		create_element(L"style", L"table-cell-properties", current->table_cell_props, context_);
	return dynamic_cast<style_table_cell_properties *>(current->table_cell_props.get());

}
style_paragraph_properties *odf_table_styles_context::get_paragraph_properties()
{
	if (current == NULL)return NULL;

	if (!current->paragraph_props)
		create_element(L"style", L"paragraph-properties", current->paragraph_props, context_);
	
	return dynamic_cast<style_paragraph_properties *>(current->paragraph_props.get());
}
style_text_properties *odf_table_styles_context::get_text_properties()
{
	if (current == NULL)return NULL;
	
	if (!current->text_props)
		create_element(L"style", L"text-properties", current->text_props, context_);
	return dynamic_cast<style_text_properties *>(current->text_props.get());
}
style_table_properties *odf_table_styles_context::get_table_properties()
{
	if (current == NULL)return NULL;
	
	if (!current->table_props)
		create_element(L"style", L"table-properties", current->table_props, context_);
	return dynamic_cast<style_table_properties *>(current->table_props.get());
}

void odf_table_styles_context::start_style(std::wstring style_name)
{
	table_format_state state;
	state.style_name = style_name;

	table_format_array_.push_back(state);

	current = &table_format_array_.back().table_;
}
void odf_table_styles_context::end_style()
{
	current = NULL;
}

void odf_table_styles_context::add_band1Horz()
{
	table_format_array_.back().band1Horz_.is = true;
	current = &table_format_array_.back().band1Horz_;
}
void odf_table_styles_context::add_band1Vert()
{
	table_format_array_.back().band1Vert_.is = true;
	current = &table_format_array_.back().band1Vert_;
}
void odf_table_styles_context::add_band2Horz()
{
	table_format_array_.back().band2Horz_.is = true;
	current = &table_format_array_.back().band2Horz_;
}
void odf_table_styles_context::add_band2Vert()
{
	table_format_array_.back().band2Vert_.is = true;
	current = &table_format_array_.back().band2Vert_;
}
void odf_table_styles_context::add_firstCol()
{
	table_format_array_.back().firstCol_.is = true;
	current = &table_format_array_.back().firstCol_;
}
void odf_table_styles_context::add_firstRow()
{
	table_format_array_.back().firstRow_.is = true;
	current = &table_format_array_.back().firstRow_;
}
void odf_table_styles_context::add_lastCol()
{
	table_format_array_.back().lastCol_.is = true;
	current = &table_format_array_.back().lastCol_;
}
void odf_table_styles_context::add_lastRow()
{
	table_format_array_.back().lastRow_.is = true;
	current = &table_format_array_.back().lastRow_;
}
void odf_table_styles_context::add_neCell()
{
	table_format_array_.back().neCell_.is = true;
	current = &table_format_array_.back().neCell_;
}
void odf_table_styles_context::add_nwCell()
{
	table_format_array_.back().nwCell_.is = true;
	current = &table_format_array_.back().nwCell_;
}
void odf_table_styles_context::add_seCell()
{
	table_format_array_.back().seCell_.is = true;
	current = &table_format_array_.back().seCell_;
}
void odf_table_styles_context::add_swCell()
{
	table_format_array_.back().swCell_.is = true;
	current = &table_format_array_.back().swCell_;
}
void odf_table_styles_context::add_wholeTable()
{
	table_format_array_.back().wholeTable_.is = true;
	current = &table_format_array_.back().wholeTable_;
}
void odf_table_styles_context::set_table_insideV(std::wstring border)
{
	table_format_array_.back().insizeV = border;
}

void odf_table_styles_context::set_table_insideH(std::wstring border)
{
	table_format_array_.back().insizeH = border;
}

bool odf_table_styles_context::start_table(std::wstring name)
{
	for (size_t i = 0; i < table_format_array_.size(); i++)
	{
		if (table_format_array_[i].style_name == name)
		{
			_use_style s = {};
			s.table_style_ = (int)i;

			current_used_.push_back(s);

			return true;
		}
	}
	return false;
}

void odf_table_styles_context::set_flags(int val)
{
	if (current_used_.empty()) return;

	current_used_.back().first_row	= (val & 0x0020) != 0 ;
	current_used_.back().first_col	= (val & 0x0080) != 0 ;
	current_used_.back().last_row	= (val & 0x0040) != 0 ;
	current_used_.back().last_col	= (val & 0x0100) != 0 ;
	current_used_.back().rows		= (val & 0x0200) == 0 ;
	current_used_.back().cols		= (val & 0x0400) == 0 ;
}

void odf_table_styles_context::set_current_dimension(int col, int row)
{
	if (current_used_.empty()) return;
	
	current_used_.back().table_col_count_ = col;
	current_used_.back().table_row_count_ = row;
}
void odf_table_styles_context::end_table()
{
	if (current_used_.empty()) return;
	
	current_used_.pop_back();
}
_CP_OPT(std::wstring)  odf_table_styles_context::get_table_insideV()
{
	table_format_state & state = table_format_array_[current_used_.back().table_style_];
	return state.insizeV;
}
_CP_OPT(std::wstring)  odf_table_styles_context::get_table_insideH()
{
	table_format_state & state = table_format_array_[current_used_.back().table_style_];
	return state.insizeH;
}
void odf_table_styles_context::get_table_cell_properties (int col, int row, style_table_cell_properties* cell_props)
{
	if (current_used_.empty()) return;
	if (cell_props == NULL) return;

	table_format_state & state = table_format_array_[current_used_.back().table_style_];
	
	int col_shift = 0;
	int row_shift = 0;

	if (state.firstCol_.is && current_used_.back().first_col)col_shift = 1;
	if (state.firstRow_.is && current_used_.back().first_row)row_shift = 1;
//------------------------------------------------------------------------------
	bool first_row = (row == 1)? true: false;
	bool first_col = (col == 1)? true: false;

	bool odd_row = ((row+row_shift)%2 != 0) ? true : false;//нечетные
	bool odd_col = ((col+col_shift)%2 != 0) ? true : false;

	bool last_row = (row == current_used_.back().table_row_count_) ? true: false; 
	bool last_col = (col == current_used_.back().table_col_count_) ? true: false; 

	bool ne = (row == 1 && col == current_used_.back().table_col_count_) ? true: false; //top right cell
	bool nw = (row == 1 && col == 1) ? true: false;						//top left cell.

	bool se = (row == current_used_.back().table_row_count_ && col == current_used_.back().table_col_count_) ? true: false; //bottom right cell
	bool sw = (row == current_used_.back().table_row_count_ && col == 1) ? true: false;						//bottom left cell.
//----------------------------------------------------------------------------------------------------------------------------------
//порядок рассмотрения - main, odd, even first, last, ne, .... col, row
					cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.table_.table_cell_props.get()));

	if (current_used_.back().cols)
	{
		if (odd_col)	cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.band1Vert_.table_cell_props.get()));
		else			cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.band2Vert_.table_cell_props.get()));
	}
	if (current_used_.back().rows)
	{
		if (odd_row)	cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.band1Horz_.table_cell_props.get()));
		else			cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.band2Horz_.table_cell_props.get()));
	}
	if (first_col && current_used_.back().first_col)cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.firstCol_.table_cell_props.get()));
	if (last_col && current_used_.back().last_col)	cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.lastCol_.table_cell_props.get()));

	if (first_row && current_used_.back().first_row)cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.firstRow_.table_cell_props.get()));
	if (last_row && current_used_.back().last_row)	cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.lastRow_.table_cell_props.get()));

/////////////////////////
	if (ne)			cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.neCell_.table_cell_props.get()));
	if (nw)			cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.nwCell_.table_cell_props.get()));

	if (se)			cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.seCell_.table_cell_props.get()));
	if (sw)			cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.swCell_.table_cell_props.get()));
}

void odf_table_styles_context::get_table_properties (style_table_properties* table_props)
{
	if (current_used_.empty()) return;
	if (table_props == NULL) return;

	table_format_state & state = table_format_array_[current_used_.back().table_style_];

	table_props->apply_from(dynamic_cast<style_table_properties *>(state.table_.table_props.get()));
}
bool odf_table_styles_context::is_paragraph_properties()
{
	if (current_used_.empty()) return false;
	
	table_format_state & state = table_format_array_[current_used_.back().table_style_];

	return state.table_.paragraph_props.get() != NULL;
}
void odf_table_styles_context::get_paragraph_properties (style_paragraph_properties* para_props)
{
	if (current_used_.empty()) return;
	if (para_props == NULL) return;

	table_format_state & state = table_format_array_[current_used_.back().table_style_];

	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.table_.paragraph_props.get()));
}
bool odf_table_styles_context::is_text_properties()
{
	if (current_used_.empty()) return false;
	
	table_format_state & state = table_format_array_[current_used_.back().table_style_];

	return state.table_.text_props.get() != NULL;
}
void odf_table_styles_context::get_text_properties (style_text_properties* text_props)
{
	if (current_used_.empty()) return;
	if (text_props == NULL) return;

	table_format_state & state = table_format_array_[current_used_.back().table_style_];

	text_props->apply_from(dynamic_cast<style_text_properties *>(state.table_.text_props.get()));
}
void odf_table_styles_context::get_table_cell_properties (style_table_cell_properties* table_cell_props)
{
	if (current_used_.empty()) return;
	if (table_cell_props == NULL) return;

	table_format_state & state = table_format_array_[current_used_.back().table_style_];

	table_cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.table_.table_cell_props.get()));
}
void odf_table_styles_context::get_text_properties (int col, int row, style_text_properties* text_props)
{
	if (current_used_.empty()) return;
	if (text_props == NULL) return;

	table_format_state & state = table_format_array_[current_used_.back().table_style_];
	
	int col_shift =0;
	int row_shift =0;

	if (state.firstCol_.is && current_used_.back().first_col)col_shift = 1;
	if (state.firstRow_.is && current_used_.back().first_row)row_shift = 1;
//------------------------------------------------------------------------------
	bool first_row = (row == 1)? true: false;
	bool first_col = (col == 1)? true: false;

	bool odd_row = ((row+row_shift)%2 != 0) ? true : false;//нечетные
	bool odd_col = ((col+col_shift)%2 != 0) ? true : false;

	bool last_row = (row == current_used_.back().table_row_count_) ? true: false; 
	bool last_col = (col == current_used_.back().table_col_count_) ? true: false; 

	bool ne = (row == 1 && col == current_used_.back().table_col_count_) ? true: false; //top right cell
	bool nw = (row == 1 && col == 1) ? true: false;						//top left cell.

	bool se = (row == current_used_.back().table_row_count_ && col == current_used_.back().table_col_count_) ? true: false; //bottom right cell
	bool sw = (row == current_used_.back().table_row_count_ && col == 1) ? true: false;						//bottom left cell
//----------------------------------------------------------------------------------------------------------------------------------
//порядок рассмотрения - main, odd, even first, last, ne, ....
					text_props->apply_from(dynamic_cast<style_text_properties *>(state.table_.text_props.get()));

	if (current_used_.back().cols)
	{	
		if (odd_col)	text_props->apply_from(dynamic_cast<style_text_properties *>(state.band1Vert_.text_props.get()));
		else			text_props->apply_from(dynamic_cast<style_text_properties *>(state.band2Vert_.text_props.get()));
	}
	
	if (current_used_.back().rows)
	{
		if (odd_row)	text_props->apply_from(dynamic_cast<style_text_properties *>(state.band1Horz_.text_props.get()));
		else			text_props->apply_from(dynamic_cast<style_text_properties *>(state.band2Horz_.text_props.get()));
	}
	
	if (first_col && current_used_.back().first_col)text_props->apply_from(dynamic_cast<style_text_properties *>(state.firstCol_.text_props.get()));
	if (last_col && current_used_.back().last_col)	text_props->apply_from(dynamic_cast<style_text_properties *>(state.lastCol_.text_props.get()));

	if (first_row && current_used_.back().first_row)text_props->apply_from(dynamic_cast<style_text_properties *>(state.firstRow_.text_props.get()));
	if (last_row && current_used_.back().last_row)	text_props->apply_from(dynamic_cast<style_text_properties *>(state.lastRow_.text_props.get()));

	if (ne)			text_props->apply_from(dynamic_cast<style_text_properties *>(state.neCell_.text_props.get()));
	if (nw)			text_props->apply_from(dynamic_cast<style_text_properties *>(state.nwCell_.text_props.get()));

	if (se)			text_props->apply_from(dynamic_cast<style_text_properties *>(state.seCell_.text_props.get()));
	if (sw)			text_props->apply_from(dynamic_cast<style_text_properties *>(state.swCell_.text_props.get()));
}
bool odf_table_styles_context::is_styled(int col, int row)
{
	table_format_state & state = table_format_array_[current_used_.back().table_style_];

	int col_shift = 0;
	int row_shift = 0;

	if (state.firstCol_.is && current_used_.back().first_col) col_shift = 1;
	if (state.firstRow_.is && current_used_.back().first_row) row_shift = 1;
	//------------------------------------------------------------------------------
	bool first_row = (row == 1) ? true : false;
	bool first_col = (col == 1) ? true : false;

	if (state.firstCol_.is && first_col) return true;
	if (state.firstRow_.is && first_row) return true;

	bool last_row = (row == current_used_.back().table_row_count_) ? true : false;
	bool last_col = (col == current_used_.back().table_col_count_) ? true : false;
	
	if (state.lastCol_.is && last_col) return true;
	if (state.lastRow_.is && last_row) return true;

	bool odd_row = ((row + row_shift) % 2 != 0) ? true : false;//нечетные
	bool odd_col = ((col + col_shift) % 2 != 0) ? true : false;

	if (state.band1Vert_.is && odd_col) return true;
	if (state.band1Horz_.is && odd_row) return true;

	if (state.band2Vert_.is && !odd_col) return true;
	if (state.band2Horz_.is && !odd_row) return true;

	bool ne = (row == 1 && col == current_used_.back().table_col_count_) ? true : false; //top right cell
	bool nw = (row == 1 && col == 1) ? true : false;						//top left cell.

	if (state.neCell_.is && ne) return true;
	if (state.nwCell_.is && nw) return true;

	bool se = (row == current_used_.back().table_row_count_ && col == current_used_.back().table_col_count_) ? true : false; //bottom right cell
	bool sw = (row == current_used_.back().table_row_count_ && col == 1) ? true : false;						//bottom left cell.

	if (state.seCell_.is && se) return true;
	if (state.swCell_.is && sw) return true;

	return false;
}
void odf_table_styles_context::get_paragraph_properties (int col, int row, style_paragraph_properties* para_props)
{
	if (current_used_.empty()) return;
	if (para_props == NULL) return;

	table_format_state & state = table_format_array_[current_used_.back().table_style_];
	
	int col_shift = 0;
	int row_shift = 0;

	if (state.firstCol_.is && current_used_.back().first_col) col_shift = 1;
	if (state.firstRow_.is && current_used_.back().first_row) row_shift = 1;
//------------------------------------------------------------------------------
	bool first_row = (row == 1)? true : false;
	bool first_col = (col == 1)? true : false;

	bool odd_row = ((row + row_shift) % 2 != 0) ? true : false;//нечетные
	bool odd_col = ((col + col_shift) % 2 != 0) ? true : false;

	bool last_row = (row == current_used_.back().table_row_count_) ? true: false; 
	bool last_col = (col == current_used_.back().table_col_count_) ? true: false; 

	bool ne = (row == 1 && col == current_used_.back().table_col_count_) ? true: false; //top right cell
	bool nw = (row == 1 && col == 1) ? true: false;						//top left cell.

	bool se = (row == current_used_.back().table_row_count_ && col == current_used_.back().table_col_count_) ? true: false; //bottom right cell
	bool sw = (row == current_used_.back().table_row_count_ && col == 1) ? true: false;						//bottom left cell.
//----------------------------------------------------------------------------------------------------------------------------------
	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.table_.paragraph_props.get()));

	if (current_used_.back().cols)
	{
		if (odd_col)	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.band1Vert_.paragraph_props.get()));
		else			para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.band2Vert_.paragraph_props.get()));
	}
	
	if (current_used_.back().cols)
	{
		if (odd_row)	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.band1Horz_.paragraph_props.get()));
		else			para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.band2Horz_.paragraph_props.get()));
	}
	
	if (first_col && current_used_.back().first_col)para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.firstCol_.paragraph_props.get()));
	if (last_col && current_used_.back().last_col)	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.lastCol_.paragraph_props.get()));

	if (first_row && current_used_.back().first_row)para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.firstRow_.paragraph_props.get()));
	if (last_row && current_used_.back().last_row)	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.lastRow_.paragraph_props.get()));

	if (ne)			para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.neCell_.paragraph_props.get()));
	if (nw)			para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.nwCell_.paragraph_props.get()));

	if (se)			para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.seCell_.paragraph_props.get()));
	if (sw)			para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.swCell_.paragraph_props.get()));

}


}
}