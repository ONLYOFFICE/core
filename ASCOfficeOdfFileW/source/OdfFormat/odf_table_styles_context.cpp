#include "precompiled_cpodf.h"

#include "logging.h"

#include "odf_table_styles_context.h"
#include "odf_conversion_context.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"

namespace cpdoccore {
namespace odf {

style_table_cell_properties *odf_table_styles_context::get_table_cell_properties()
{
	if (current == NULL)return NULL;

	if (!current->table_cell_props)
		create_element(L"style", L"table-cell-properties",current->table_cell_props, context_);
	return dynamic_cast<style_table_cell_properties *>(current->table_cell_props.get());

}
style_paragraph_properties	*odf_table_styles_context::get_paragraph_properties()
{
	if (current == NULL)return NULL;

	if (!current->paragraph_props)
		create_element(L"style", L"paragraph-properties",current->paragraph_props, context_);
	return dynamic_cast<style_paragraph_properties *>(current->paragraph_props.get());
}
style_text_properties		*odf_table_styles_context::get_text_properties()
{
	if (current == NULL)return NULL;
	
	if (!current->text_props)
		create_element(L"style", L"text-properties",current->text_props, context_);
	return dynamic_cast<style_text_properties *>(current->text_props.get());
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
	current = &table_format_array_.back().band1Horz_;
}
void odf_table_styles_context::add_band1Vert()
{
	current = &table_format_array_.back().band1Vert_;
}
void odf_table_styles_context::add_band2Horz()
{
	current = &table_format_array_.back().band2Horz_;
}
void odf_table_styles_context::add_band2Vert()
{
	current = &table_format_array_.back().band2Vert_;
}
void odf_table_styles_context::add_firstCol()
{
	current = &table_format_array_.back().firstCol_;
}
void odf_table_styles_context::add_firstRow()
{
	current = &table_format_array_.back().firstRow_;
}
void odf_table_styles_context::add_lastCol()
{
	current = &table_format_array_.back().band1Horz_;
}
void odf_table_styles_context::add_lastRow()
{
	current = &table_format_array_.back().lastRow_;
}
void odf_table_styles_context::add_neCell()
{
	current = &table_format_array_.back().neCell_;
}
void odf_table_styles_context::add_nwCell()
{
	current = &table_format_array_.back().nwCell_;
}
void odf_table_styles_context::add_seCell()
{
	current = &table_format_array_.back().seCell_;
}
void odf_table_styles_context::add_swCell()
{
	current = &table_format_array_.back().swCell_;
}
void odf_table_styles_context::add_wholeTable()
{
	current = &table_format_array_.back().wholeTable_;
}
bool odf_table_styles_context::set_current_style(std::wstring name)
{
	for (long i=0; i < table_format_array_.size(); i++)
	{
		if (table_format_array_[i].style_name == name)
		{
			current_table_style_ = i;
			return true;
		}
	}
	return false;
}
void odf_table_styles_context::set_current_dimension(int col, int row)
{
	current_table_col_count_ = col;
	current_table_row_count_ = row;
}

void odf_table_styles_context::get_table_cell_properties (int col, int row, style_table_cell_properties* cell_props)
{
	if (current_table_style_ < 0)	return;
	if (cell_props == NULL)			return;
//------------------------------------------------------------------------------
	bool first_row = (row == 1)? true: false;
	bool first_col = (col == 1)? true: false;

	bool odd_row = (row%2 != 0) ? true : false;//нечетные
	bool odd_col = (col%2 != 0) ? true : false;

	bool even_row = (row%2 != 0) ? true : false;//четные
	bool even_col = (col%2 != 0) ? true : false;

	bool last_row = (row == current_table_row_count_) ? true: false; 
	bool last_col = (col == current_table_col_count_) ? true: false; 

	bool ne = (row == 1 && col == current_table_col_count_) ? true: false; //top right cell
	bool nw = (row == 1 && col == 1) ? true: false;						//top left cell.

	bool se = (row == current_table_row_count_ && col == current_table_col_count_) ? true: false; //bottom right cell
	bool sw = (row == current_table_row_count_ && col == 1) ? true: false;						//bottom left cell.

//----------------------------------------------------------------------------------------------------------------------------------
//порядок рассмотрения - main, odd, even first, last, ne, .... col, row

	table_format_state & state = table_format_array_[current_table_style_];

					cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.table_.table_cell_props.get()));

	if (odd_col)	cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.band1Vert_.table_cell_props.get()));
	if (even_col)	cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.band2Vert_.table_cell_props.get()));
	
	if (odd_row)	cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.band1Horz_.table_cell_props.get()));
	if (even_row)	cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.band2Horz_.table_cell_props.get()));
	
	if (first_col)	cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.firstCol_.table_cell_props.get()));
	if (last_col)	cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.lastCol_.table_cell_props.get()));

	if (first_row)	cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.firstRow_.table_cell_props.get()));
	if (last_row)	cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.lastRow_.table_cell_props.get()));

	if (ne)			cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.neCell_.table_cell_props.get()));
	if (nw)			cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.nwCell_.table_cell_props.get()));

	if (se)			cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.seCell_.table_cell_props.get()));
	if (sw)			cell_props->apply_from(dynamic_cast<style_table_cell_properties *>(state.swCell_.table_cell_props.get()));
}
void odf_table_styles_context::get_text_properties (int col, int row, style_text_properties* text_props)
{
	if (current_table_style_ < 0) return;
	if (text_props == NULL)			return;
//------------------------------------------------------------------------------
	bool first_row = (row == 1)? true: false;
	bool first_col = (col == 1)? true: false;

	bool odd_row = (row%2 != 0) ? true : false;//нечетные
	bool odd_col = (col%2 != 0) ? true : false;

	bool even_row = (row%2 != 0) ? true : false;//четные
	bool even_col = (col%2 != 0) ? true : false;

	bool last_row = (row == current_table_row_count_) ? true: false; 
	bool last_col = (col == current_table_col_count_) ? true: false; 

	bool ne = (row == 1 && col == current_table_col_count_) ? true: false; //top right cell
	bool nw = (row == 1 && col == 1) ? true: false;						//top left cell.

	bool se = (row == current_table_row_count_ && col == current_table_col_count_) ? true: false; //bottom right cell
	bool sw = (row == current_table_row_count_ && col == 1) ? true: false;						//bottom left cell.

//----------------------------------------------------------------------------------------------------------------------------------
//порядок рассмотрения - main, odd, even first, last, ne, ....

	table_format_state & state = table_format_array_[current_table_style_];

					text_props->apply_from(dynamic_cast<style_text_properties *>(state.table_.text_props.get()));

	if (odd_col)	text_props->apply_from(dynamic_cast<style_text_properties *>(state.band1Vert_.text_props.get()));
	if (even_col)	text_props->apply_from(dynamic_cast<style_text_properties *>(state.band2Vert_.text_props.get()));
	
	if (odd_row)	text_props->apply_from(dynamic_cast<style_text_properties *>(state.band1Horz_.text_props.get()));
	if (even_row)	text_props->apply_from(dynamic_cast<style_text_properties *>(state.band2Horz_.text_props.get()));
	
	if (first_col)	text_props->apply_from(dynamic_cast<style_text_properties *>(state.firstCol_.text_props.get()));
	if (last_col)	text_props->apply_from(dynamic_cast<style_text_properties *>(state.lastCol_.text_props.get()));

	if (first_row)	text_props->apply_from(dynamic_cast<style_text_properties *>(state.firstRow_.text_props.get()));
	if (last_row)	text_props->apply_from(dynamic_cast<style_text_properties *>(state.lastRow_.text_props.get()));

	if (ne)			text_props->apply_from(dynamic_cast<style_text_properties *>(state.neCell_.text_props.get()));
	if (nw)			text_props->apply_from(dynamic_cast<style_text_properties *>(state.nwCell_.text_props.get()));

	if (se)			text_props->apply_from(dynamic_cast<style_text_properties *>(state.seCell_.text_props.get()));
	if (sw)			text_props->apply_from(dynamic_cast<style_text_properties *>(state.swCell_.text_props.get()));
}
void odf_table_styles_context::get_paragraph_properties (int col, int row, style_paragraph_properties* para_props)
{
	if (current_table_style_ < 0) return;
	if (para_props == NULL)			return;
//------------------------------------------------------------------------------
	bool first_row = (row == 1)? true: false;
	bool first_col = (col == 1)? true: false;

	bool odd_row = (row%2 != 0) ? true : false;//нечетные
	bool odd_col = (col%2 != 0) ? true : false;

	bool even_row = (row%2 != 0) ? true : false;//четные
	bool even_col = (col%2 != 0) ? true : false;

	bool last_row = (row == current_table_row_count_) ? true: false; 
	bool last_col = (col == current_table_col_count_) ? true: false; 

	bool ne = (row == 1 && col == current_table_col_count_) ? true: false; //top right cell
	bool nw = (row == 1 && col == 1) ? true: false;						//top left cell.

	bool se = (row == current_table_row_count_ && col == current_table_col_count_) ? true: false; //bottom right cell
	bool sw = (row == current_table_row_count_ && col == 1) ? true: false;						//bottom left cell.

//----------------------------------------------------------------------------------------------------------------------------------
//порядок рассмотрения - main, odd, even first, last, ne, ....

	table_format_state & state = table_format_array_[current_table_style_];

					para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.table_.paragraph_props.get()));

	if (odd_col)	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.band1Vert_.paragraph_props.get()));
	if (even_col)	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.band2Vert_.paragraph_props.get()));
	
	if (odd_row)	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.band1Horz_.paragraph_props.get()));
	if (even_row)	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.band2Horz_.paragraph_props.get()));
	
	if (first_col)	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.firstCol_.paragraph_props.get()));
	if (last_col)	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.lastCol_.paragraph_props.get()));

	if (first_row)	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.firstRow_.paragraph_props.get()));
	if (last_row)	para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.lastRow_.paragraph_props.get()));

	if (ne)			para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.neCell_.paragraph_props.get()));
	if (nw)			para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.nwCell_.paragraph_props.get()));

	if (se)			para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.seCell_.paragraph_props.get()));
	if (sw)			para_props->apply_from(dynamic_cast<style_paragraph_properties *>(state.swCell_.paragraph_props.get()));

}


}
}