#include "precompiled_cpodf.h"
#include "logging.h"

#include "ods_table_state.h"
#include "ods_conversion_context.h"

#include "table.h"

#include "styles.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_graphic_properties.h"


namespace cpdoccore {
namespace odf {

ods_table_state::ods_table_state(ods_conversion_context & Context, office_element_ptr & elm): context_(Context)
{     
	office_table_ = elm; 
	columns_count_=0;
	current_table_row_ =0;
	current_table_column_ =0;
}

void ods_table_state::set_name(std::wstring name)
{
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	tableName_ = name;
	table->table_table_attlist_.table_name_ = name;
}

void ods_table_state::convert()
{
	if (office_table_ == NULL)return;

	//??? 

}

void ods_table_state::set_table_style(office_element_ptr & elm)
{	
	office_table_style_ = dynamic_cast<style*>(elm.get());

	if (!office_table_style_)return;

	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;
	
	table->table_table_attlist_.table_style_name_ = office_table_style_->style_name_;
	//потом в принципе и по имени можно будет связать(найти)


}
void ods_table_state::add_column(office_element_ptr & elm, int repeated,office_element_ptr & style_elm)
{
	office_table_->add_child_element(elm);

	std::wstring style_name;

	odf::style* style = dynamic_cast<odf::style*>(style_elm.get());
	if (style)style_name = style->style_name_;

	ods_element_state state = {elm, repeated,style_name, style_elm};
  
	columns_count_ += repeated;
    columns_.push_back(state);

	table_table_column* column = dynamic_cast<table_table_column*>(columns_.back().elm.get());
	if (column == NULL)return;

	if (style_name.length()>0) column->table_table_column_attlist_.table_style_name_ = style_ref(style_name);
	column->table_table_column_attlist_.table_number_columns_repeated_ = repeated;
	
}
void ods_table_state::set_column_default_cell_style(std::wstring & style_name)
{
	table_table_column* column = dynamic_cast<table_table_column*>(columns_.back().elm.get());
	if (column == NULL)return;

	column->table_table_column_attlist_.table_default_cell_style_name_ = style_ref(style_name);
}

void ods_table_state::set_column_width(int width)//cm, pt ???
{
	odf::style* style = dynamic_cast<odf::style*>(columns_.back().style_elm.get());
	if (!style)return;		

	style_table_column_properties * column_properties = style->style_content_.get_style_table_column_properties();
 	if (column_properties == NULL)return; //error ????

	column_properties->style_table_column_properties_attlist_.style_column_width_ = length(width/2.,length::cm);
}
void ods_table_state::set_column_optimal_width(bool val)
{
	odf::style* style = dynamic_cast<odf::style*>(columns_.back().style_elm.get());
	if (!style)return;		

	style_table_column_properties * column_properties = style->style_content_.get_style_table_column_properties();
 	if (column_properties == NULL)return; //error ????

	column_properties->style_table_column_properties_attlist_.style_use_optimal_column_width_ = val;

}
unsigned int ods_table_state::columns_count() const
{
    return columns_count_;
}
void ods_table_state::add_row(office_element_ptr & elm, int repeated,office_element_ptr & style_elm)
{
    current_table_column_ = 0; 
    current_table_row_+=repeated;

	office_table_->add_child_element(elm);

	std::wstring style_name;

	odf::style* style = dynamic_cast<odf::style*>(style_elm.get());
	if (style)style_name = style->style_name_;

	ods_element_state state = {elm, repeated,style_name, style_elm};
  
    rows_.push_back(state);

	table_table_row* row = dynamic_cast<table_table_row*>(rows_.back().elm.get());
	if (row == NULL)return;

	if (style_name.length()>0) row->table_table_row_attlist_.table_style_name_ = style_ref(style_name);
	row->table_table_row_attlist_.table_number_rows_repeated_ = repeated;

}
void ods_table_state::set_row_hidden(bool Val)
{
	table_table_row* row = dynamic_cast<table_table_row*>(rows_.back().elm.get());
	if (row == NULL)return;

	row->table_table_row_attlist_.table_visibility_ = table_visibility(table_visibility::Collapse);
}
void ods_table_state::set_row_optimal_height(bool val)
{
	odf::style* style = dynamic_cast<odf::style*>(rows_.back().style_elm.get());
	if (!style)return;		

	style_table_row_properties * row_properties = style->style_content_.get_style_table_row_properties();
 	if (row_properties == NULL)return; //error ????

	row_properties->style_table_row_properties_attlist_.style_use_optimal_row_height_ = val;

}

void ods_table_state::set_row_height(double height)
{
	odf::style* style = dynamic_cast<odf::style*>(rows_.back().style_elm.get());
	if (!style)return;		

	style_table_row_properties * row_properties = style->style_content_.get_style_table_row_properties();
 	if (row_properties == NULL)return; //error ????

	row_properties->style_table_row_properties_attlist_.style_row_height_ = length(height,length::pt);
}
int ods_table_state::current_column() const
{
    return current_table_column_;
}

int ods_table_state::current_row() const
{
    return current_table_row_;
}

void ods_table_state::set_row_default_cell_style(std::wstring & style_name)
{
	table_table_row* row = dynamic_cast<table_table_row*>(rows_.back().elm.get());
	if (row == NULL)return;

	row->table_table_row_attlist_.table_default_cell_style_name_ = style_ref(style_name);
}

office_element_ptr  & ods_table_state::current_row_element()
{
	if (rows_.size()>0)
		return rows_.back().elm;
	else
	{
	}
}

void ods_table_state::start_cell(office_element_ptr & elm, office_element_ptr & style_elm)
{
	current_row_element()->add_child_element(elm);
	
	table_table_cell* cell = dynamic_cast<table_table_cell*>(elm.get());
	if (cell == NULL)return;
	
	odf::style* style = dynamic_cast<odf::style*>(rows_.back().style_elm.get());
	if (style)
	{
		cell->table_table_cell_attlist_.table_style_name_=	style->style_name_;
	}

}
void ods_table_state::end_cell()
{
}
void ods_table_state::add_default_cell(office_element_ptr &  elm, int repeated)
{
	current_row_element()->add_child_element(elm);
	
	table_table_cell* cell = dynamic_cast<table_table_cell*>(elm.get());
	if (cell == NULL)return;

	cell->table_table_cell_attlist_.table_number_columns_repeated_ = repeated;
}
}
}
