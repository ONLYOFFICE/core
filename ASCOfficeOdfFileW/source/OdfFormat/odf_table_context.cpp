#include "precompiled_cpodf.h"
#include "logging.h"

#include <boost/foreach.hpp>

#include <iostream>

#include "odf_conversion_context.h"

#include "odf_table_context.h" //easy(lite) case ods_table_state/context - make based???
#include "styles.h"

#include "table.h"

#include "style_table_properties.h"
//#include "style_text_properties.h"
//#include "style_paragraph_properties.h"
//#include "style_graphic_properties.h"


namespace cpdoccore 
{
namespace odf
{
	struct odf_column_state : odf_element_state
	{
		std::vector<office_element_ptr>	spanned_row_cell;
	};
	struct odf_table_state
	{
		odf_table_state()
		{
			current_row = 0;
			current_column = 0;
			count_header_row = 0;
		}
		std::vector<odf_element_state> rows;
		std::vector<odf_column_state> columns;
		std::vector<odf_element_state> cells;

		odf_element_state table;

		__int32 current_row;
		__int32 current_column;

		__int32 count_header_row;

	};

class odf_table_context::Impl
{
public:
	Impl(odf_conversion_context *odf_context) :odf_context_(odf_context)
    {	

	} 

	odf_table_state & current_table()	{return tables_.back();}
	bool empty () {return tables_.size() >0 ? false : true; }

	void start_table(odf_table_state & state) {tables_.push_back(state);}

	void end_table() {if (tables_.size() > 0) tables_.pop_back();}

	odf_style_context	* styles_context() {return odf_context_->styles_context();}

	//std::wstring default_cell_style_name_ ; // нету :( придетс€ накатывать на обычные
	odf_conversion_context *odf_context_; 

private:
	std::vector<odf_table_state> tables_;//типо current level ... дл€ вложенных таблиц

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

void odf_table_context::start_table(office_element_ptr &elm, bool styled)
{
	table_table * table = dynamic_cast<table_table *>(elm.get());
	if (!table)return;	
	
	odf_table_state state;
	state.table.elm = elm;

	if (styled)
	{
		odf_style_state * style_state = impl_->styles_context()->last_state(style_family::Table);
		if (style_state)
		{
			state.table.style_elm = style_state->get_office_element();
			state.table.style_name = style_state->get_name();
			table->table_table_attlist_.table_style_name_ = state.table.style_name;		
			
			//дл€ красивой отрисовки в редакторах - разрешим объеденить стили пересекающихс€ обрамлений 
			style_table_properties *table_properties = style_state->get_table_properties();
			table_properties->table_format_properties_.table_border_model_ = odf::border_model(odf::border_model::Collapsing);
		}
	}

	impl_->start_table(state);
}
void odf_table_context::end_table()
{
	//последние объединенные по вертикали €чейки .. 
	for (long i =0 ; i < impl_->current_table().columns.size(); i++)
	{
		impl_->current_table().current_column = i+1;
		set_cell_row_span_restart();
	}
	impl_->end_table(); 
}
void odf_table_context::start_row(office_element_ptr &elm, bool styled)
{
	if (impl_->empty()) return;

	table_table_row * row = dynamic_cast<table_table_row *>(elm.get());;
	if (!row)return;
	
	odf_element_state state;

	state.elm = elm;
	if (styled)
	{
		odf_style_state * style_state = impl_->styles_context()->last_state(style_family::TableRow);
		if (style_state)
		{
			state.style_elm = style_state->get_office_element();
			state.style_name = style_state->get_name();
			row->table_table_row_attlist_.table_style_name_ = state.style_name;		
		}
	}


	impl_->current_table().rows.push_back(state);

	impl_->current_table().current_column =0;
	impl_->current_table().current_row ++;
}

void odf_table_context::end_row()
{
	if (impl_->empty()) return;
	
	//for (int i = impl_->current_table().current_column ; i< impl_->current_table().columns.size() ; i++)
	//{
	//	office_element_ptr cell; //потом на default ???
	//	create_element(L"table", L"table-cell",cell , impl_->odf_context_);
	//	start_cell(cell,false);
	//	end_cell();
	//}
	impl_->current_table().current_column =0;
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
		odf_style_state * style_state = impl_->styles_context()->last_state(style_family::TableColumn);
		if (style_state)
		{
			state.style_elm = style_state->get_office_element();
			state.style_name = style_state->get_name();
			column->table_table_column_attlist_.table_style_name_ = state.style_name;		
		}
	}

	impl_->current_table().columns.push_back(state);

}
void odf_table_context::set_column_width(double width)
{
	if (impl_->empty()) return;
	if (impl_->current_table().columns.size() < 1)return;

	style *style_ = dynamic_cast<style*>(impl_->current_table().columns.back().style_elm.get());

	if (style_ == NULL) return;
	style_table_column_properties *properties = style_->style_content_.get_style_table_column_properties();
	if (properties == NULL) return;

	if (width >= 0)
	{
		properties->style_table_column_properties_attlist_.style_column_width_ = length(length(width,length::pt).get_value_unit(length::cm),length::cm);
		properties->style_table_column_properties_attlist_.style_use_optimal_column_width_ = false;
	}
	else
		properties->style_table_column_properties_attlist_.style_use_optimal_column_width_ = true;
}
int odf_table_context::current_column ()
{
	if (impl_->empty()) return 0;
	
	return impl_->current_table().current_column;
}
int odf_table_context::count_column ()
{
	if (impl_->empty()) return 0;

	return impl_->current_table().columns.size();

}
void odf_table_context::start_cell(office_element_ptr &elm, bool styled)
{
	if (impl_->empty()) return;

	table_table_cell * cell = dynamic_cast<table_table_cell *>(elm.get());;
	table_covered_table_cell * covered_cell = dynamic_cast<table_covered_table_cell *>(elm.get());;
	if (!cell && !covered_cell)return;
	
	odf_element_state state;

	state.elm = elm;
	if (styled && cell)
	{
		odf_style_state * style_state = impl_->styles_context()->last_state(style_family::TableCell);
		if (style_state)
		{
			state.style_elm = style_state->get_office_element();
			state.style_name = style_state->get_name();
			cell->table_table_cell_attlist_.table_style_name_ = state.style_name;		
		}
	}

	impl_->current_table().cells.push_back(state);

	impl_->current_table().current_column++;
}
void odf_table_context::set_cell_column_span(int spanned)
{
	if (impl_->empty()) return;
	if (impl_->current_table().cells.size() < 1)return;
	if (spanned < 1)return;

	table_table_cell * cell = dynamic_cast<table_table_cell *>(impl_->current_table().cells.back().elm.get());;
	if (!cell)return;

	cell->table_table_cell_attlist_extra_.table_number_columns_spanned_ = spanned;

	//impl_->current_table().current_column += spanned-1;
				
}

void odf_table_context::set_cell_row_span()
{
	int col = impl_->current_table().current_column-1;
	odf_column_state & state = impl_->current_table().columns[col];

	state.spanned_row_cell.push_back(impl_->current_table().cells.back().elm);
}

void odf_table_context::set_cell_row_span_restart()
{
	int col = impl_->current_table().current_column-1;
	odf_column_state & state = impl_->current_table().columns[col];

	int sz = state.spanned_row_cell.size();
	
	if (sz  > 1)
	{
		table_table_cell * cell = dynamic_cast<table_table_cell *>(state.spanned_row_cell[0].get());
		if (!cell)return;

		cell->table_table_cell_attlist_extra_.table_number_rows_spanned_ = sz;
	}
	state.spanned_row_cell.clear();
	state.spanned_row_cell.push_back(impl_->current_table().cells.back().elm);

}

void odf_table_context::end_cell()
{
	if (impl_->empty()) return;
}
}
}