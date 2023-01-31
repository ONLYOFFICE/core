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

#include <iostream>

#include "odf_conversion_context.h"

#include "odf_table_context.h" //easy(lite) case ods_table_state/context - make based???
#include "styles.h"

#include "table.h"

#include "style_table_properties.h"
#include "style_graphic_properties.h"
#include "style_text_properties.h"

//#include "style_paragraph_properties.h"


namespace cpdoccore 
{

	using namespace odf_types;

namespace odf_writer
{
	struct odf_column_state : odf_element_state
	{
		std::vector<office_element_ptr>	spanned_row_cell;
	};
	struct odf_row_state : odf_element_state
	{
		std::vector<office_element_ptr>	spanned_column_cell;
	};
	struct odf_table_state
	{
		std::vector<odf_row_state>		rows;
		std::vector<odf_column_state>	columns;
		std::vector<odf_element_state>	cells;

		odf_element_state table;

        int current_row = 0;
        int current_column = 0;

        int current_count_rows = 0;
        int count_header_row = 0;

		_CP_OPT(size_t)count_cols;
		_CP_OPT(size_t)count_rows;
		
		bool styled = false;

		double table_width = 0;
		double table_height = 0;

		std::wstring default_cell_properties;
		
		std::wstring first_row_cell_properties;
		std::wstring first_col_cell_properties;
		std::wstring band_row_cell_properties;
		std::wstring band_col_cell_properties;
		std::wstring last_row_cell_properties;
		std::wstring last_col_cell_properties;

		_CP_OPT(std::wstring) border_inside_v_;
		_CP_OPT(std::wstring) border_inside_h_;

	};

class odf_table_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) :odf_context_(odf_context)
    {	
		default_column_width = boost::none;
		default_row_height = boost::none;
		optimal_column_width = false;
	} 

	odf_table_state & current_table()	{return tables_.back();}
	bool empty () {return (false == tables_.empty()) ? false : true; }

	void start_table(odf_table_state & state) {tables_.push_back(state);}

	void end_table() 
	{
		if (false == tables_.empty()) tables_.pop_back(); 
		
		default_column_width	= boost::none;  // todo .. in level ???
		default_row_height		= boost::none;
		optimal_column_width	= false;
		
		default_cell_properties.clear();
	}

	odf_style_context *styles_context() {return odf_context_->styles_context();}

	odf_conversion_context *odf_context_; 

	_CP_OPT(double)	default_row_height;
	_CP_OPT(double)	default_column_width;
	bool			optimal_column_width;

	std::wstring default_cell_properties; // для предустановки ..

private:
	std::vector<odf_table_state> tables_;//типо current level ... для вложенных таблиц

};

////////////////////////////////////////////////////////////////////////////
odf_table_context::odf_table_context(odf_conversion_context *odf_context)  
	: impl_(new  odf_table_context::Impl(odf_context))
	//for embedded need styles_context ??? todooo
{
}

odf_table_context::~odf_table_context()
{
}

void odf_table_context::set_table_styled(bool val)
{
	impl_->current_table().styled = val;
}
bool odf_table_context::is_styled(int row, int col)
{
	if (impl_->empty()) return false;
	if (!impl_->current_table().styled) return false;

	return impl_->styles_context()->table_styles().is_styled(row, col);
}
bool odf_table_context::is_styled()
{
	if (impl_->empty()) return false;
	return impl_->current_table().styled;
}
void odf_table_context::set_table_size(size_t cols, size_t rows)
{
	if (cols > 0)
		impl_->current_table().count_cols = cols;
	if (rows > 0)
		impl_->current_table().count_rows = rows;
}
void odf_table_context::start_table(office_element_ptr &elm, bool styled)
{
	table_table * table = dynamic_cast<table_table *>(elm.get());
	if (!table) return;	
	
	odf_table_state state;
	
	state.table.elm					= elm;
	state.default_cell_properties	= impl_->default_cell_properties; //write in row default cell props

	if (styled)
	{
		odf_style_state_ptr style_state = impl_->styles_context()->last_state(style_family::Table);
		if (style_state)
		{
			state.table.style_elm = style_state->get_office_element();
			state.table.style_name = style_state->get_name();
			
			table->attlist_.table_style_name_ = state.table.style_name;		
		}
	}
	impl_->default_cell_properties = L"";
	impl_->start_table(state);
}
void odf_table_context::end_table()
{
	//последние объединенные ячейки ..
	if (impl_->current_table().columns.empty())
	{
	}
	for (size_t i = 0 ; i < impl_->current_table().columns.size(); i++)
	{
		impl_->current_table().current_column = (int)i + 1;
		set_cell_row_span_restart();
	}
	for (size_t i = 0 ; i < impl_->current_table().rows.size(); i++)
	{
		impl_->current_table().current_row = (int)i + 1;
		set_cell_column_span_restart();
	}
	style * style_ = dynamic_cast<style *>(impl_->current_table().table.style_elm.get());
	if (style_)
	{
		if (impl_->current_table().table_width > 0)
		{
			style_table_properties * table_props = style_->content_.get_style_table_properties();
			if (table_props)
			{
				table_props->content_.style_width_ = length(length(impl_->current_table().table_width,length::pt).get_value_unit(length::cm),length::cm);
			}
		}
	}
	impl_->end_table(); 
}
void odf_table_context::start_row(office_element_ptr &elm, bool styled)
{
	if (impl_->empty()) return;

	table_table_row * row = dynamic_cast<table_table_row *>(elm.get());
	if (!row)return;
	
	odf_row_state state;

	state.elm = elm;

	if (styled)
	{
		odf_style_state_ptr style_state = impl_->styles_context()->last_state(style_family::TableRow);
		if (style_state)
		{
			state.style_elm		= style_state->get_office_element();
			state.style_name	= style_state->get_name();
			
			row->attlist_.table_style_name_ = state.style_name;		
		}
	}
	std::wstring default_cell_props = impl_->current_table().default_cell_properties;

	if (impl_->current_table().current_row == 0 && false == impl_->current_table().first_row_cell_properties.empty())
	{
		default_cell_props = impl_->current_table().first_row_cell_properties;
	}
	else if (impl_->current_table().count_rows && impl_->current_table().current_row == *impl_->current_table().count_rows - 1 && false == impl_->current_table().last_row_cell_properties.empty())
	{
		default_cell_props = impl_->current_table().last_row_cell_properties;
	}	
	else if (impl_->current_table().current_row % 2 != 0 && false == impl_->current_table().band_row_cell_properties.empty())
	{
		default_cell_props = impl_->current_table().band_row_cell_properties;
	}

	if (false == default_cell_props.empty())
		row->attlist_.table_default_cell_style_name_ = default_cell_props;


	impl_->current_table().rows.push_back(state);

	impl_->current_table().current_column = 0;
	impl_->current_table().current_row++;
}

void odf_table_context::end_row()
{
	if (impl_->empty()) return;
	
	set_cell_column_span_restart();

	//for (int i = impl_->current_table().current_column ; i < impl_->current_table().columns.size() ; i++)
	//{
	//	office_element_ptr cell; //потом на default ???
	//	create_element(L"table", L"table-cell",cell , impl_->odf_context_);
	//	start_cell(cell,false);
	//	end_cell();
	//}
	impl_->current_table().current_column = 0;
}


bool odf_table_context::empty()
{
	return impl_->empty();
}
void odf_table_context::add_column(office_element_ptr &elm, bool styled)
{
	if (impl_->empty()) return;
	
	table_table_column * column = dynamic_cast<table_table_column *>(elm.get());;
	if (!column)return;

	odf_column_state state;

	state.elm = elm;
	if (styled)
	{
		odf_style_state_ptr style_state = impl_->styles_context()->last_state(style_family::TableColumn);
		if (style_state)
		{
			state.style_elm = style_state->get_office_element();
			state.style_name = style_state->get_name();
			column->attlist_.table_style_name_ = state.style_name;		
		}
	}

	impl_->current_table().columns.push_back(state);

	if ((!impl_->current_table().count_cols) || (*impl_->current_table().count_cols < impl_->current_table().columns.size()))
	{
		impl_->current_table().count_cols = impl_->current_table().columns.size();
	}
}

void odf_table_context::set_table_inside_v(_CP_OPT(std::wstring) border)
{
	if (impl_->empty()) return;
	impl_->current_table().border_inside_v_ = border;
}

void odf_table_context::set_table_inside_h(_CP_OPT(std::wstring) border)
{
	if (impl_->empty()) return;
	impl_->current_table().border_inside_h_ = border;
}
_CP_OPT(std::wstring) odf_table_context::get_table_inside_v()
{
	_CP_OPT(std::wstring) none;
	if (impl_->empty()) return none; 
	return impl_->current_table().border_inside_v_;
}
_CP_OPT(std::wstring) odf_table_context::get_table_inside_h()
{
	_CP_OPT(std::wstring) none;
	if (impl_->empty()) return none; 
	return impl_->current_table().border_inside_h_;
}
void odf_table_context::set_default_cell_properties(const std::wstring &style_name)
{
	if (impl_->empty()) impl_->default_cell_properties					= style_name;
	else				impl_->current_table().default_cell_properties	= style_name;
}
void odf_table_context::set_first_row_cell_properties(const std::wstring &style_name)
{
	if (false == impl_->empty())
		impl_->current_table().first_row_cell_properties	= style_name;
}
void odf_table_context::set_first_col_cell_properties(const std::wstring &style_name)
{
	if (false == impl_->empty())
		impl_->current_table().first_col_cell_properties	= style_name;
}
void odf_table_context::set_band_row_cell_properties(const std::wstring &style_name)
{
	if (false == impl_->empty())
		impl_->current_table().band_row_cell_properties	= style_name;
}
void odf_table_context::set_band_col_cell_properties(const std::wstring &style_name)
{
	if (false == impl_->empty())
		impl_->current_table().band_col_cell_properties	= style_name;
}
void odf_table_context::set_last_row_cell_properties(const std::wstring &style_name)
{
	if (false == impl_->empty())
		impl_->current_table().last_row_cell_properties	= style_name;
}
void odf_table_context::set_last_col_cell_properties(const std::wstring &style_name)
{
	if (false == impl_->empty())
		impl_->current_table().last_col_cell_properties	= style_name;
}
_CP_OPT(double) odf_table_context::get_table_width()
{
	if (impl_->empty()) return boost::none;
	else return impl_->current_table().table_width;
}
_CP_OPT(double) odf_table_context::get_table_height()
{
	if (impl_->empty()) return boost::none;
	else return impl_->current_table().table_height;
}
std::wstring odf_table_context::get_default_cell_properties()
{
	if (impl_->empty()) return impl_->default_cell_properties;
	
	std::wstring default_cell_props = impl_->current_table().default_cell_properties;

	if (impl_->current_table().current_row == 1 && false == impl_->current_table().first_row_cell_properties.empty())
	{
		default_cell_props = impl_->current_table().first_row_cell_properties;
	}
	else if (impl_->current_table().count_rows && impl_->current_table().current_row == *impl_->current_table().count_rows && false == impl_->current_table().last_row_cell_properties.empty())
	{
		default_cell_props = impl_->current_table().last_row_cell_properties;
	}	
	else if (impl_->current_table().current_row % 2 == 0 && false == impl_->current_table().band_row_cell_properties.empty())
	{
		default_cell_props = impl_->current_table().band_row_cell_properties;
	}

	return default_cell_props;
}
std::wstring odf_table_context::get_column_cell_properties()
{
	if (impl_->empty()) return L"";
	
	std::wstring default_cell_props;

	if (impl_->current_table().current_column == 0 && false == impl_->current_table().first_col_cell_properties.empty())
	{
		default_cell_props = impl_->current_table().first_col_cell_properties;
	}
	else if (impl_->current_table().count_cols && impl_->current_table().current_column == *impl_->current_table().count_cols - 1 && false == impl_->current_table().last_col_cell_properties.empty())
	{
		default_cell_props = impl_->current_table().last_col_cell_properties;
	}	
	else if (impl_->current_table().current_column % 2 == 0 && false == impl_->current_table().band_col_cell_properties.empty())
	{
		default_cell_props = impl_->current_table().band_col_cell_properties;
	}

	return default_cell_props;
}
void odf_table_context::set_default_row_height(double height)
{
	impl_->default_row_height = height;
}
void odf_table_context::set_default_column_width(double width)
{
	impl_->default_column_width = width;
}
void odf_table_context::set_optimal_column_width(bool val)
{
	impl_->optimal_column_width = val;
}
void odf_table_context::set_column_optimal(bool val)
{
	if (impl_->empty()) return;
	if (impl_->current_table().columns.size() < 1)return;

	style *style_ = dynamic_cast<style*>(impl_->current_table().columns.back().style_elm.get());

	if (style_ == NULL) return;
	style_table_column_properties *properties = style_->content_.get_style_table_column_properties();
	if (properties == NULL) return;

	properties->style_table_column_properties_attlist_.style_use_optimal_column_width_ = val;
}

void odf_table_context::change_current_column_width(double width)
{
	if (impl_->empty()) return;
	
	int index = impl_->current_table().current_column ;
	if (index < 0) return;	
	
	while(index >= impl_->current_table().columns.size())
	{
		office_element_ptr elm;
		create_element(L"table", L"table-column", elm, impl_->odf_context_);

		impl_->styles_context()->create_style(L"", style_family::TableColumn, true, false, -1);

		add_column(elm, true);

		impl_->current_table().table.elm->add_child_element(elm);
	}

	style *style_ = dynamic_cast<style*>(impl_->current_table().columns[index].style_elm.get());

	if (style_ == NULL) return;
	style_table_column_properties *properties = style_->content_.get_style_table_column_properties();
	if (properties == NULL) return;

	length length_ = length(length(width, length::pt).get_value_unit(length::cm), length::cm);
	
	if (!properties->style_table_column_properties_attlist_.style_column_width_)
		properties->style_table_column_properties_attlist_.style_column_width_ = length_;
	else
	{
		double old_width = properties->style_table_column_properties_attlist_.style_column_width_->get_value_unit(length::pt);
		if (old_width < width/* && width < impl_->odf_context_->page_layout_context()->current_page_width_*/ && old_width < 9)
			properties->style_table_column_properties_attlist_.style_column_width_ = length_;
	}
}

void odf_table_context::set_column_width(double width)
{
	if (impl_->empty()) return;
	if (impl_->current_table().columns.empty())return;

	style *style_ = dynamic_cast<style*>(impl_->current_table().columns.back().style_elm.get());
	style_table_column_properties *properties = style_ ? style_->content_.get_style_table_column_properties() : NULL;

	if (width > 0)
	{
		if (properties)
		{
			properties->style_table_column_properties_attlist_.style_column_width_ = length(length(width,length::pt).get_value_unit(length::cm),length::cm);
			//properties->style_table_column_properties_attlist_.style_rel_column_width_ = length(length(width,length::pt).get_value_unit(length::cm),length::cm);

			if (impl_->optimal_column_width)
			{
				properties->style_table_column_properties_attlist_.style_use_optimal_column_width_ = true;
			}
		}

		impl_->current_table().table_width += width;
	}
	else
	{
		if (properties)
		{
			properties->style_table_column_properties_attlist_.style_use_optimal_column_width_ = true;

			if (impl_->default_column_width)
			{
				properties->style_table_column_properties_attlist_.style_column_width_ = length(length(impl_->default_column_width.get(), length::pt).get_value_unit(length::cm), length::cm);
				//properties->style_table_column_properties_attlist_.style_rel_column_width_ = length(length(impl_->current_table().table_width,length::pt).get_value_unit(length::cm),length::cm);
			}
		}
		impl_->current_table().table_width += impl_->default_column_width.get_value_or(0);
	}
}
void odf_table_context::set_row_height(double height)
{
	if (impl_->empty()) return;

	if (height > 0)
	{
		//style *style_ = dynamic_cast<odf_writer::style*>(rows_.back().style_elm.get());

		//if (style_)
		//{
		//	style_table_row_properties *properties = style_->content_.get_style_table_row_properties();
		//	if (properties)
		//	{			
		//		//properties->style_table_column_properties_attlist_.style_column_width_ = length(length(width,length::pt).get_value_unit(length::cm),length::cm);
		//	}
		//}
		impl_->current_table().table_height += height;

		if (!impl_->default_row_height)
			impl_->default_row_height = height;
	}
	else
	{
		impl_->current_table().table_height += impl_->default_row_height.get_value_or(0);
	}
}

int odf_table_context::current_column ()
{
	if (impl_->empty()) return 0;
	
	return impl_->current_table().current_column;
}
int odf_table_context::current_row ()
{
	if (impl_->empty()) return 0;
	
	return impl_->current_table().current_row;
}
int odf_table_context::count_columns ()
{
	if (impl_->empty()) return 0;

	return (int)impl_->current_table().columns.size();
}
int odf_table_context::count_rows ()
{
	if (impl_->empty()) return 0;

	return impl_->current_table().current_count_rows;
}

void odf_table_context::start_cell(office_element_ptr &elm, bool styled)
{
	if (impl_->empty()) return;

	table_table_cell*			cell			= dynamic_cast<table_table_cell *>(elm.get());;
	table_covered_table_cell*	covered_cell	= dynamic_cast<table_covered_table_cell *>(elm.get());
	
	if (!cell && !covered_cell)return;
	
	std::wstring def_props = get_default_cell_properties();
	
	odf_element_state state(elm);

	if (styled && cell)
	{
		odf_style_state_ptr style_state = impl_->styles_context()->last_state(style_family::TableCell);
		if (style_state)
		{
			state.style_elm		= style_state->get_office_element();
			state.style_name	= style_state->get_name();
			
			cell->attlist_.table_style_name_ = state.style_name;		
		}
	}
	else if (false == def_props.empty() && cell)
	{
		cell->attlist_.table_style_name_ = def_props;		
	}
//------------------------------------------------------------------------------
	int row = impl_->current_table().current_row - 1;
	odf_row_state & state_row = impl_->current_table().rows[row];
	
	if (cell && state_row.spanned_column_cell.size() > 1)
	{
		table_table_cell * cell = dynamic_cast<table_table_cell *>(state_row.spanned_column_cell[0].get());
		if (!cell)return;

		cell->attlist_extra_.table_number_columns_spanned_ = (unsigned int)state_row.spanned_column_cell.size();	
		state_row.spanned_column_cell.clear();
	}

	impl_->current_table().cells.push_back(state);

	impl_->current_table().current_column++;
}
void odf_table_context::set_cell_column_span_restart()
{
	int row = impl_->current_table().current_row - 1;
	odf_row_state & state = impl_->current_table().rows[row];
	
	unsigned int sz = (unsigned int)state.spanned_column_cell.size();	
	if (sz  > 1)
	{
		table_table_cell * cell = dynamic_cast<table_table_cell *>(state.spanned_column_cell[0].get());
		if (!cell)return;

		cell->attlist_extra_.table_number_columns_spanned_ = sz;
	}
	state.spanned_column_cell.clear();
}
void odf_table_context::set_cell_column_span(int spanned)
{
	if (impl_->empty()) return;
	if (impl_->current_table().cells.size() < 1)return;
	
	if (spanned > 0)
	{
		table_table_cell * cell = dynamic_cast<table_table_cell *>(impl_->current_table().cells.back().elm.get());
		if (cell)
			cell->attlist_extra_.table_number_columns_spanned_ = spanned;
	}
	else
	{
		int row = impl_->current_table().current_row - 1;
		odf_row_state & state = impl_->current_table().rows[row];

		if (state.spanned_column_cell.empty() && impl_->current_table().cells.size() > 1 )
		{
			state.spanned_column_cell.push_back(impl_->current_table().cells[impl_->current_table().cells.size() - 2].elm);
		}
		state.spanned_column_cell.push_back(impl_->current_table().cells.back().elm);
	}
}

void odf_table_context::set_cell_row_span(int spanned)
{
	if (spanned > 0)
	{
		table_table_cell * cell = dynamic_cast<table_table_cell *>(impl_->current_table().cells.back().elm.get());
		if (cell)
			cell->attlist_extra_.table_number_rows_spanned_ = spanned;
	}
	else
	{
		int col = impl_->current_table().current_column - 1;
		
		if (col < (int)impl_->current_table().columns.size())
		{
			odf_column_state & state = impl_->current_table().columns[col];

			state.spanned_row_cell.push_back(impl_->current_table().cells.back().elm);
		}
	}
}

void odf_table_context::set_cell_row_span_restart()
{
	int col = impl_->current_table().current_column - 1;

	if (col >= (int)impl_->current_table().columns.size())
		return;

	odf_column_state & state = impl_->current_table().columns[col];

	unsigned int sz = (unsigned int)state.spanned_row_cell.size();
	
	if (sz  > 1)
	{
		table_table_cell * cell = dynamic_cast<table_table_cell *>(state.spanned_row_cell[0].get());
		if (!cell)return;

		cell->attlist_extra_.table_number_rows_spanned_ = sz;
	}
	state.spanned_row_cell.clear();

	if (false == impl_->current_table().cells.empty())
	{
		state.spanned_row_cell.push_back(impl_->current_table().cells.back().elm);
	}

}

void odf_table_context::end_cell()
{
	if (impl_->empty()) return;
}
}
}
