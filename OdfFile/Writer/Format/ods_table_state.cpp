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

#include <boost/algorithm/string.hpp>
#include <boost/date_time.hpp>

#include "../../Formulas/formulasconvert.h"

#include "ods_table_state.h"
#include "odf_text_context.h"
#include "ods_conversion_context.h"

#include "table.h"
#include "office_annotation.h"
#include "calcext_elements.h"
#include "table_data_pilot_tables.h"
#include "styles.h"
#include "paragraph_elements.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"

namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer {

int ods_table_state::current_table_column_ = 0;
int ods_table_state::current_table_row_ = 0;

int ods_table_state::tmp_column_ =0;
int ods_table_state::tmp_row_ =0;

namespace utils//////////////////////////////////////////// ОБЩАЯ хрень .. вытащить что ли в utils ???

{
	std::wstring convert_date(int date)
	{
		boost::gregorian::date date_ = boost::gregorian::date(1900, 1, 1) + boost::gregorian::date_duration(date - 2);

		std::wstring date_str;

		try
		{
			date_str = boost::lexical_cast<std::wstring>(date_.year())
				+ L"-" +
				(date_.month() < 10 ? L"0" : L"") + boost::lexical_cast<std::wstring>(date_.month().as_number())
				+ L"-" +
				(date_.day() < 10 ? L"0" : L"") + boost::lexical_cast<std::wstring>(date_.day());
		}
		catch (...)
		{
			date_str = date;
		}
		return date_str;
	}
	std::wstring convert_date(const std::wstring & oox_date)
	{
		int iDate = 0;

		try
		{
			iDate = boost::lexical_cast<int>(oox_date);
		}catch(...)
		{
			return oox_date;
		}
		return convert_date(iDate);
	}
	std::wstring convert_time(double dTime)
	{
		//12H15M42S
		int hours = 0, minutes = 0;
		double sec = 0;

		boost::posix_time::time_duration day(24, 0, 0);

		double millisec = day.total_milliseconds() * dTime;


		sec = millisec / 1000.;
		hours = (int)(sec / 60. / 60.);
		minutes = (int)((sec - (hours * 60 * 60)) / 60.);
		sec = sec - (hours * 60 + minutes) * 60.;

		int sec1 = (int)sec;

		std::wstring time_str =
			(hours < 10 ? L"0" : L"") + boost::lexical_cast<std::wstring>(hours)
			//+ std::wstring(L"H") +
			+ std::wstring(L":") +
			(minutes < 10 ? L"0" : L"") + boost::lexical_cast<std::wstring>(minutes)
			//+ std::wstring(L"M") +
			+ std::wstring(L":") +
			(sec1 < 10 ? L"0" : L"") + boost::lexical_cast<std::wstring>(sec1);
			//+ std::wstring(L"S");

		return time_str;
	}
	std::wstring convert_date_time(const std::wstring & oox_time, office_value_type::type & type)
	{
		double dDateTime = 0;

		try
		{
			dDateTime = boost::lexical_cast<double>(oox_time);
		}
		catch (...)
		{
			return oox_time;
		}
		int nDate = (int)dDateTime;
		double dTime = (dDateTime - nDate);

		std::wstring sDate, sTime;
		if (dTime > 0)
		{
			sTime = convert_time(dTime);
		}
		if (nDate > 0)
		{
			sDate = convert_date(nDate);
			type = office_value_type::Date;
		}
		else 
			type = office_value_type::Time;
		
		// "1899-12-31T05:37:46.66569
		return sDate + (sTime.empty() ? L"" : L"T" + sTime);
	}

	std::wstring convert_time(const std::wstring & oox_time)
	{
		double dTime = 0;
		
		try
		{		
			dTime = boost::lexical_cast<double>(oox_time);
		}catch(...)
		{
			return oox_time;
		}
		//PT12H15M42S
		return std::wstring(L"PT") + convert_time(dTime);
	}
};

///////////////////////////////////////////////////////////////
static formulasconvert::oox2odf_converter formulas_converter_table;

ods_table_state::ods_table_state(odf_conversion_context * Context, office_element_ptr & elm) : 
		context_(Context), controls_context_(Context)
{     
	drawing_context_ = boost::make_shared<odf_drawing_context>(context_);
	office_table_ = elm; 

	current_table_row_ = 0;
	current_table_column_ = 0;

	current_level_.push_back(office_table_);

//default 	dimension

	dimension_columns = 64;
	dimension_row = 64;


	defaut_row_height_ = 9;
	defaut_column_width_ = 28.34467120181406 * 1.674;// 
}

void ods_table_state::set_table_name(std::wstring name)
{
	office_table_name_ = name;
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	table->attlist_.table_name_ = name;
}

void ods_table_state::set_table_master_page(std::wstring name)
{
	if (!office_table_style_)return;

	office_table_style_->style_master_page_name_ = name;
}
void ods_table_state::set_table_protection(bool Val)
{
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	table->attlist_.table_protected_ = true;
	create_element(L"loext", L"table-protection", table->table_protection_, context_);
}
void ods_table_state::set_table_protection_insert_columns(bool Val)
{
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	table_table_protection *prot = dynamic_cast<table_table_protection*>(table->table_protection_.get());
	if (!prot) return;

	prot->insert_columns = Val;
}
void ods_table_state::set_table_protection_insert_rows(bool Val)
{
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	table_table_protection *prot = dynamic_cast<table_table_protection*>(table->table_protection_.get());
	if (!prot) return;

	prot->insert_rows = Val;
}
void ods_table_state::set_table_protection_delete_columns(bool Val)
{
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	table_table_protection *prot = dynamic_cast<table_table_protection*>(table->table_protection_.get());
	if (!prot) return;

	prot->delete_columns = Val;
}
void ods_table_state::set_table_protection_delete_rows(bool Val)
{
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	table_table_protection *prot = dynamic_cast<table_table_protection*>(table->table_protection_.get());
	if (!prot) return;

	prot->delete_rows = Val;
}
void ods_table_state::set_table_protection_protected_cells(bool Val)
{
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	table_table_protection *prot = dynamic_cast<table_table_protection*>(table->table_protection_.get());
	if (!prot) return;

	prot->select_protected_cells = Val;
}
void ods_table_state::set_table_protection_unprotected_cells(bool Val)
{
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	table_table_protection *prot = dynamic_cast<table_table_protection*>(table->table_protection_.get());
	if (!prot) return;

	prot->select_unprotected_cells = Val;
}
void ods_table_state::set_table_hidden(bool Val)
{
	if (!office_table_style_)return;

	style_table_properties *table_properties = office_table_style_->content_.get_style_table_properties();
	if (table_properties == NULL)return;

	table_properties->content_.table_display_ = !Val;

}
void ods_table_state::set_table_rtl(bool Val)
{
	if (!office_table_style_)return;

	style_table_properties *table_properties = office_table_style_->content_.get_style_table_properties();
	if (table_properties == NULL)return;

	table_properties->content_.common_writing_mode_attlist_.style_writing_mode_ = writing_mode(writing_mode::RlTb);

}
void ods_table_state::set_table_print_ranges(const std::wstring &ranges)
{
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	table->attlist_.table_print_ranges_ = ranges;
}
void ods_table_state::set_table_tab_color(_CP_OPT(color) & _color)
{
	if (!office_table_style_)return;

	style_table_properties *table_properties = office_table_style_->content_.get_style_table_properties();
	if (table_properties == NULL)return;

	table_properties->content_.tableooo_tab_color_ = _color;
}
void ods_table_state::set_table_style(office_element_ptr & elm)
{	
	office_table_style_ = dynamic_cast<style*>(elm.get());

	if (!office_table_style_)return;

	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;
	
	table->attlist_.table_style_name_ = office_table_style_->style_name_;
	//потом в принципе и по имени можно будет связать(найти)
}
void ods_table_state::start_group(office_element_ptr & elm)
{
	current_level_.back()->add_child_element(elm);
	current_level_.push_back(elm);
}

void ods_table_state::end_group()
{
	current_level_.pop_back();
}
void ods_table_state::start_headers(office_element_ptr & elm)
{
	current_level_.back()->add_child_element(elm);
	current_level_.push_back(elm);
}
void ods_table_state::end_headers()
{
	current_level_.pop_back();
}
void ods_table_state::add_column_break(int val)
{
	column_breaks_.push_back(val + 1);
}
void ods_table_state::add_row_break(int val)
{
	row_breaks_.push_back(val + 1);
}
void ods_table_state::add_column(office_element_ptr & elm, unsigned int repeated,office_element_ptr & style_elm)
{
	current_level_.back()->add_child_element(elm);

	std::wstring style_name;

	odf_writer::style* style = dynamic_cast<odf_writer::style*>(style_elm.get());
	if (style)style_name = style->style_name_;

    ods_element_state state(elm, repeated, style_name, style_elm, defaut_column_width_, current_level_.size());
  
	//if (repeated > 10000) repeated = 1024;//????

	current_table_column_ += repeated;
    columns_.push_back(state);

	table_table_column* column = dynamic_cast<table_table_column*>(columns_.back().elm.get());
	if (column == NULL)return;

	if (false == style_name.empty()) column->attlist_.table_style_name_ = style_name;
	column->attlist_.table_number_columns_repeated_ = repeated;
	
}
void ods_table_state::set_column_default_cell_style(std::wstring & style_name)
{
	if (style_name.length() < 1)return;

	table_table_column* column = dynamic_cast<table_table_column*>(columns_.back().elm.get());
	if (column == NULL)return;

	column->attlist_.table_default_cell_style_name_ = style_name;

	columns_.back().cell_style_name = style_name;
}
std::wstring ods_table_state::get_column_default_cell_style(int column)
{
    int curr=0;

    for (size_t i=0; i < columns_.size(); i++)
	{
		if (curr + (int)columns_[i].repeated < column + 1)continue;
		else
		{
			return columns_[i].cell_style_name;
		}
	}
	return L"";
}
void ods_table_state::set_column_width(double width)//pt
{
	odf_writer::style* style = dynamic_cast<odf_writer::style*>(columns_.back().style_elm.get());
	if (!style)return;		

	style_table_column_properties * column_properties = style->content_.get_style_table_column_properties();
 	if (column_properties == NULL)return; //error ????

	columns_.back().size = width; //pt

	column_properties->style_table_column_properties_attlist_.style_column_width_ = length(length(width,length::pt).get_value_unit(length::cm),length::cm);
}
void ods_table_state::set_column_optimal_width(bool val)
{
	odf_writer::style* style = dynamic_cast<odf_writer::style*>(columns_.back().style_elm.get());
	if (!style)return;		

	style_table_column_properties * column_properties = style->content_.get_style_table_column_properties();
 	if (column_properties == NULL)return; //error ????

	column_properties->style_table_column_properties_attlist_.style_use_optimal_column_width_ = val;

}
void ods_table_state::set_column_hidden(bool val)
{
	table_table_column* column = dynamic_cast<table_table_column*>(columns_.back().elm.get());
	if (column == NULL)return;

	column->attlist_.table_visibility_ = table_visibility(table_visibility::Collapse);
}
void ods_table_state::set_table_dimension(int col, int row)
{
	if (col<1 || row <1 )return;

	if (dimension_columns < col)	dimension_columns = col + 1;
	if (dimension_row < row)		dimension_row = row + 1;
}

void ods_table_state::add_row(office_element_ptr & elm, unsigned int repeated, office_element_ptr & style_elm)
{
    current_table_column_	= 0; 
    current_table_row_		+= repeated;
	current_covered_cols_	= 0;

	for (size_t i = 0; i < current_covered_rows_.size(); ++i)
	{
		current_covered_rows_[i].count_rows--;
		if (current_covered_rows_[i].count_rows <= 0)
		{
			current_covered_rows_.erase(current_covered_rows_.begin() + i);
			i--;
		}
	}

	current_level_.back()->add_child_element(elm);

	std::wstring style_name;

	odf_writer::style* style = dynamic_cast<odf_writer::style*>(style_elm.get());
	if (style)style_name = style->style_name_;

    ods_element_state state(elm, repeated, style_name, style_elm, defaut_row_height_ , current_level_.size());
  
    rows_.push_back(state);

	table_table_row* row = dynamic_cast<table_table_row*>(rows_.back().elm.get());
	if (row == NULL)return;

	if (false == style_name.empty()) row->attlist_.table_style_name_ = style_name;
	row->attlist_.table_number_rows_repeated_ = repeated;

	row_default_cell_style_name_ = L"";

}
void ods_table_state::add_row_repeated()
{
	table_table_row* row = dynamic_cast<table_table_row*>(rows_.back().elm.get());
	if (row == NULL)return;

	unsigned int t = rows_.back().repeated;
	rows_.back().repeated++;
	current_table_row_++;
	
	for (size_t i = 0; i < current_covered_rows_.size(); ++i)
	{
		current_covered_rows_[i].count_rows--;
		if (current_covered_rows_[i].count_rows <= 0)
		{
			current_covered_rows_.erase(current_covered_rows_.begin() + i);
			i--;
		}
	}
	row->attlist_.table_number_rows_repeated_ = rows_.back().repeated;
}
void ods_table_state::set_row_hidden(bool Val)
{
	table_table_row* row = dynamic_cast<table_table_row*>(rows_.back().elm.get());
	if (row == NULL)return;

	row->attlist_.table_visibility_ = table_visibility(table_visibility::Collapse);
}
void ods_table_state::set_row_optimal_height(bool val)
{
	odf_writer::style* style = dynamic_cast<odf_writer::style*>(rows_.back().style_elm.get());
	if (!style)return;		

	style_table_row_properties * row_properties = style->content_.get_style_table_row_properties();
 	if (row_properties == NULL)return; //error ????

	row_properties->style_table_row_properties_attlist_.style_use_optimal_row_height_ = val;
}

void ods_table_state::set_row_height(double height)
{
	odf_writer::style* style = dynamic_cast<odf_writer::style*>(rows_.back().style_elm.get());
	if (!style)return;		

	style_table_row_properties * row_properties = style->content_.get_style_table_row_properties();
 	if (row_properties == NULL)return; //error ????

	rows_.back().size = height;//pt

	row_properties->style_table_row_properties_attlist_.style_row_height_ = length(height,length::pt);//length(length(height,length::pt).get_value_unit(length::cm),length::cm);
}

bool ods_table_state::is_cell_hyperlink()
{
	if (cells_.empty())return false;
	return cells_.back().hyperlink_idx >= 0 ? true : false;
}
bool ods_table_state::is_cell_comment()
{
	if (cells_.empty())return false;
	return cells_.back().comment_idx >= 0 ? true : false;
}
bool ods_table_state::is_cell_data_validation()
{
	if (cells_.empty()) return false;
	return cells_.back().data_validation_name.empty() ? true : false;
}
int ods_table_state::is_cell_hyperlink(int col, int row)
{
    for (size_t i = 0; i < hyperlinks_.size(); i++)
	{
		if (hyperlinks_[i].col == col && hyperlinks_[i].row == row)
		{
            return  (int)i;
		}
	}
	return -1;
}
std::wstring ods_table_state::is_cell_data_validation(int col, int row, unsigned int repeate_col, data_validation_state::_ref & ref)
{
	for (size_t i = 0; i < data_validations_.size(); i++)
	{
		if (data_validations_[i].in_ref(col, row, repeate_col, ref))
		{
            return data_validations_[i].name;
		}
	}
	return L"";
}
int ods_table_state::is_row_validation(int row, int & repeate_row)
{
	for (size_t i = 0; i < data_validations_.size(); i++)
	{
		data_validation_state::_ref ref;
		if (data_validations_[i].in_row(row, repeate_row, ref))
		{
			repeate_row = (std::min)(ref.row_end, row + repeate_row - 1) - (std::max)(row, ref.row_start) + 1;
			
			if (ref.row_start > row) row = ref.row_start;
			return  row;
		}
	}
	return -1;
}
int ods_table_state::is_cell_comment(int col, int row, unsigned int repeate_col)
{
	for (size_t i = 0; i < comments_.size(); i++)
	{
		if ((comments_[i].col < col + (int)repeate_col && comments_[i].col >= col) && comments_[i].row == row && comments_[i].used == false)
		{
            return  (int)i;
		}
	}
	return -1;
}
int ods_table_state::is_row_comment(int row, int repeate_row)
{
	for (size_t i = 0; i < comments_.size(); i++)
	{
		if (comments_[i].row < row + repeate_row && comments_[i].row >= row && comments_[i].used == false)
		{
			return comments_[i].row;
		}
	}
	return -1;
}
int ods_table_state::current_column() const
{
    return current_table_column_;
}


int ods_table_state::current_row() const
{
    return current_table_row_;
}

unsigned int ods_table_state::get_last_row_repeated ()
{
	return rows_.empty() ? 1 : rows_.back().repeated;
}

void ods_table_state::set_row_default_cell_style(std::wstring & style_name)
{
	row_default_cell_style_name_= style_name;	//обязательно нужно определить default-style (table_cell)!!!
	
	//if (style_name.length() < 1) return;

	//table_table_row* row = dynamic_cast<table_table_row*>(rows_.back().elm.get());
	//if (row == NULL)return;

	//row->attlist_.table_default_cell_style_name_ = style_name;
}

office_element_ptr  & ods_table_state::current_row_element()
{
	if (false == rows_.empty())
		return rows_.back().elm;
	else
		throw;
}
office_element_ptr  & ods_table_state::current_cell_element()
{
	if (false == cells_.empty())
		return cells_.back().elm;
	else
		throw;
}
ods_hyperlink_state & ods_table_state::current_hyperlink()
{
	if ((false == cells_.empty() && !hyperlinks_.empty()) && (cells_.back().hyperlink_idx >= 0) )
	{
		return hyperlinks_[cells_.back().hyperlink_idx];
	}
	else
		throw;
}

void ods_table_state::start_cell(office_element_ptr & elm, office_element_ptr & style_elm)
{
	current_row_element()->add_child_element(elm);
	
	std::wstring style_name;

	odf_writer::style* style = dynamic_cast<odf_writer::style*>(style_elm.get());
	if (style)	style_name = style->style_name_;
	else		style_name = row_default_cell_style_name_;

	table_table_cell* cell = dynamic_cast<table_table_cell*>(elm.get());
	if (cell && !style_name.empty() && style_name != get_column_default_cell_style(current_column())) 
	{
		cell->attlist_.table_style_name_ =	style_name;
	}
	table_covered_table_cell* covered_cell = dynamic_cast<table_covered_table_cell*>(elm.get());
	if (covered_cell && !style_name.empty() && style_name != get_column_default_cell_style(current_column())) 
	{
		covered_cell->attlist_.table_style_name_ =	style_name;
	}
	ods_cell_state state;

	state.empty = true;
	state.elm = elm;  state.repeated = 1;  state.style_name = style_name; state.style_elm = style_elm;
    state.row = current_table_row_;  state.col = current_table_column_ + 1;

	data_validation_state::_ref ref;
	std::wstring validation_name = is_cell_data_validation(state.col, state.row, 1, ref);

	state.hyperlink_idx			= is_cell_hyperlink(state.col, state.row);
	state.comment_idx			= is_cell_comment(state.col, state.row);
	state.data_validation_name	= validation_name;

	current_table_column_ +=  state.repeated;  
    cells_.push_back(state);
	
	if (current_covered_cols_ > 0 && covered_cell)
		current_covered_cols_--;
}

void ods_table_state::set_cell_format_value(office_value_type::type value_type)
{
	if (cells_.empty())return;
	if (value_type == office_value_type::Custom) return;	//general .. need detect

	table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
	if (cell == NULL)return;

	common_value_and_type_attlist cell_type;
	cell_type.office_value_type_ = office_value_type(value_type);

	cell->attlist_.common_value_and_type_attlist_ = cell_type;

}
void ods_table_state::set_cell_type(int type)
{
	if (cells_.empty())return;

	table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
	if (cell == NULL)return;

	_CP_OPT(office_value_type) cell_type;
	switch (type)
	{
	case 0:		cell_type = office_value_type(office_value_type::Boolean);
		break;
	case 1:		cell_type = office_value_type(office_value_type::Date);
		break;
	case 4:		cell_type = office_value_type(office_value_type::Float);
		break;
	case 2:// error-type
	case 3:// inline
	case 5:// shared
	case 6:// обычная строка
		cell_type = office_value_type(office_value_type::String);
		break;
	}
	if (cell_type)
	{
		if (!cell->attlist_.common_value_and_type_attlist_)
		{
			cell->attlist_.common_value_and_type_attlist_ = common_value_and_type_attlist();
		}
		cell->attlist_.common_value_and_type_attlist_->office_value_type_ = cell_type;
	}
}
void ods_table_state::add_definded_expression(office_element_ptr & elm)
{
	if (!table_defined_expressions_)
	{
		create_element(L"table", L"named-expressions", table_defined_expressions_, context_);
		office_table_->add_child_element(table_defined_expressions_);
	}
	if (!table_defined_expressions_)return;
	table_defined_expressions_->add_child_element(elm);
}
void ods_table_state::add_hyperlink(const std::wstring & ref,int col, int row, const std::wstring & link, const std::wstring & location)
{
	ods_hyperlink_state state;
	state.row = row;  state.col = col; state.ref = ref; 

	state.link = link;		
	
	if (link.empty())
	{
		state.link += L"#" + formulas_converter_table.convert_named_ref(location);
		state.bLocation = true;
	}
	else
	{
		state.link = link + (location.empty() ? L"" : (L"#" + location));
	}

	hyperlinks_.push_back(state);
}
void ods_table_state::start_comment(int col, int row, std::wstring & author)
{
	ods_comment_state state;

	state.row = row;  state.col = col; state.author = author;	
	create_element(L"office", L"annotation", state.elm, context_);

	office_annotation * annotation = dynamic_cast<office_annotation*>(state.elm.get());
	if (!annotation)return;	

	context_->styles_context()->create_style(L"", style_family::Graphic, true, false, -1);		
	
	office_element_ptr & style_elm = context_->styles_context()->last_state()->get_office_element();
	state.graphic_properties = context_->styles_context()->last_state()->get_graphic_properties();
	
	style* style_ = dynamic_cast<style*>(style_elm.get());
	if (style_)
	{
		annotation->attr_.draw_style_name_ = style_->style_name_;
	}

	comments_.push_back(state);
}
void ods_table_state::set_comment_color(const std::wstring & color)
{
	if (color.empty()) return;
	if (comments_.empty()) return;
	if (!comments_.back().graphic_properties) return;

	comments_.back().graphic_properties->common_draw_fill_attlist_.draw_fill_color_ = L"#" + color;
	comments_.back().graphic_properties->common_draw_fill_attlist_.draw_fill_ = odf_types::draw_fill::solid;
}
void ods_table_state::set_comment_visible(bool val)
{
	if (comments_.empty()) return;

	office_annotation * annotation = dynamic_cast<office_annotation*>(comments_.back().elm.get());
	if (!annotation)return;	

	annotation->attr_.display_ = val;
}
void ods_table_state::set_comment_rect(double l, double t, double w, double h)
{
	if (comments_.empty())return;
	
	if (l < 0.1 && t < 0.1 && w < 0.1 && h < 0.1)return;

	office_annotation * annotation = dynamic_cast<office_annotation*>(comments_.back().elm.get());
	if (!annotation)return;		

	annotation->attr_.svg_y_		= length(t/10.,length::cm);
	annotation->attr_.svg_x_		= length(l/10.,length::cm);
	annotation->attr_.svg_width_	= length(w/10.,length::cm);
	annotation->attr_.svg_height_	= length(h/10.,length::cm);
}
void ods_table_state::end_comment(odf_text_context *text_context)
{
	if (text_context == NULL || comments_.size() <1 )return;

	for (size_t i = 0; i < text_context->text_elements_list_.size(); i++)
	{
		if (text_context->text_elements_list_[i].level ==0 && comments_.back().elm)
		{
			comments_.back().elm->add_child_element(text_context->text_elements_list_[i].elm);
		}
	}
	if (comments_.back().author.length() > 0 && comments_.back().elm)
	{
		office_element_ptr dc_elm;
		create_element(L"dc", L"creator", dc_elm, context_);
		if (dc_elm)
		{
			dc_elm->add_text(comments_.back().author);
			comments_.back().elm->add_child_element(dc_elm);
		}
	}
}

void ods_table_state::check_spanned_cells()
{
	for (std::map<int, std::map<int, _spanned_info>>::iterator it = map_merged_cells.begin(); it != map_merged_cells.end(); ++it)
	{
		for (std::map<int, _spanned_info>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
		{	
			int start_row = it->first;
			int end_row = it->first + jt->second.spanned_rows;

			int start_col = jt->first;
			int end_col = jt->first + jt->second.spanned_cols;

			for (size_t i = 0; i < cells_.size(); ++i) //todooo cells_ vector -> map by row
			{
				if (cells_[i].row > end_row) break;

				if (cells_[i].row >= start_row)
				{
					if (cells_[i].col >= start_col)
					{
						table_table_cell* cell_elm = dynamic_cast<table_table_cell*>(cells_[i].elm.get());
						if (cell_elm == NULL)break;

						cell_elm->attlist_extra_.table_number_columns_spanned_ = jt->second.spanned_cols;
						cell_elm->attlist_extra_.table_number_rows_spanned_ = jt->second.spanned_rows;

						break;
					}
				}
			}
		}
	}
}

void ods_table_state::set_merge_cells(int start_col, int start_row, int end_col, int end_row)
{
	if (end_col - start_col < 0) return;
	if (end_row - start_row < 0) return;

	_spanned_info info;
	
	info.spanned_cols = end_col - start_col + 1;
	info.spanned_rows = end_row - start_row + 1;

	//if (info.spanned_cols > 10000) info.spanned_cols = 1024;

	std::map<int, std::map<int, _spanned_info>>::iterator pFindRow = map_merged_cells.find(start_row);

	if (pFindRow == map_merged_cells.end())
	{
		std::map<int, _spanned_info> mapCols;
		mapCols.insert(std::make_pair(start_col, info));
		
		map_merged_cells.insert(std::make_pair(start_row, mapCols));
	}
	else
	{
		std::map<int, _spanned_info>::iterator pFindCol = pFindRow->second.find(start_col);
		if (pFindCol == pFindRow->second.end())
		{
			pFindRow->second.insert(std::make_pair(start_col, info));
			
		}
		//else нереально pFindCol->second.insert(info);
	}
}
bool ods_table_state::isSpannedCell(int col, int row, int &spanned_cols, int &spanned_rows )
{
	spanned_cols = spanned_rows = 0;

	std::map<int, std::map<int, _spanned_info>>::iterator pFindRow = map_merged_cells.find(row);

	if (pFindRow != map_merged_cells.end())
	{
		std::map<int, _spanned_info>::iterator pFindCol = pFindRow->second.find(col);
		if (pFindCol != pFindRow->second.end())
		{
			spanned_cols = pFindCol->second.spanned_cols;
			spanned_rows = pFindCol->second.spanned_rows;

			current_covered_cols_ = spanned_cols - 1;

			if (spanned_rows > 1 && current_covered_cols_ > 0)
			{
				_covered_info info;
				info.start_col = current_table_column_;
				info.count_cols = spanned_cols;
				
				info.count_rows = spanned_rows - 1;
				current_covered_rows_.push_back(info);
			}

			pFindRow->second.erase(pFindCol);

			if (pFindRow->second.empty())
			{
				map_merged_cells.erase(pFindRow);
			}

			return true;
		}
	}
	return false;
}
bool ods_table_state::isCoveredCell( int col, int repeated_cols)
{
	if (current_covered_cols_ > 0) return true;

	for (size_t i = 0; i < current_covered_rows_.size(); i++)
	{
		if (current_covered_rows_[i].start_col <= col && col < current_covered_rows_[i].start_col + current_covered_rows_[i].count_cols)
		{
			current_covered_cols_ = current_covered_rows_[i].count_cols - (col - current_covered_rows_[i].start_col);
			return true;
		}
	}

	return false;
}
void ods_table_state::set_cell_spanned(int spanned_cols, int spanned_rows)
{
	table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
	if (cell == NULL)return;

	cell->attlist_extra_.table_number_columns_spanned_ = spanned_cols;
	cell->attlist_extra_.table_number_rows_spanned_ = spanned_rows;
}
void ods_table_state::set_cell_formula(std::wstring & formula)
{
	if (formula.empty())return;

	ods_conversion_context* ods_context = dynamic_cast<ods_conversion_context*>(context_);
	
	//test external link
	bool bExternal = !ods_context->externals_.empty();
	boost::wregex re(L"([\\[]\\d+[\\]])+");

	while(bExternal)
	{
		boost::wsmatch result;
		bExternal = boost::regex_search(formula, result, re);
		if (!bExternal) break;

		
		std::wstring refExternal = result[1].str();
		int idExternal = XmlUtils::GetInteger(refExternal.substr(1, refExternal.length() - 1)) - 1;

		bExternal = (idExternal >= 0 && idExternal < (int)ods_context->externals_.size());

		while(idExternal >= 0 && idExternal < (int)ods_context->externals_.size())
		{
			size_t pos = formula.find(refExternal);
			if (pos == std::wstring::npos)
				break;

			std::wstring new_formula; 
			
			if (pos > 0 && formula[pos - 1] == L'\'')
			{
				new_formula = formula.substr(0, pos - 1);
				new_formula += L"'EXTERNALREF" + ods_context->externals_[idExternal].ref + L"'#";
				new_formula += L"'";
			}
			else
			{
				new_formula = formula.substr(0, pos);
				new_formula += L"'EXTERNALREF" + ods_context->externals_[idExternal].ref + L"'#";
			}
			pos += refExternal.length();
			new_formula += formula.substr(pos, formula.length() - pos);
			formula = new_formula;
		}
	}

	std::wstring odfFormula = formulas_converter_table.convert_formula(formula);

	XmlUtils::replace_all(odfFormula, L"EXTERNALREF", L"file://");//снятие экранирования

	if ((false == table_parts_.empty()) && (std::wstring::npos != odfFormula.find(L"[")))
	{
		for (size_t i = 0; i < table_parts_.size(); i++)
		{
			if (table_parts_[i].in_ref(current_table_column_, current_table_row_))
			{
				for (size_t j = 0; j < table_parts_[i].columns.size(); j ++)
				{
					std::wstring name = table_parts_[i].name + L"[" + table_parts_[i].columns[j].first + L"]";
					//Таблица1[ Сумма за кв. 3 ]

					XmlUtils::replace_all(odfFormula, name, table_parts_[i].columns[j].second);
				}
				break;
			}
		}
	}
	
	table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
	if (cell == NULL)return;

	cell->attlist_.table_formula_ = odfFormula;
	cells_.back().empty = false;
}

std::wstring ods_table_state::replace_cell_row(boost::wsmatch const & what)
{
    if (what[1].matched)
	{
		std::wstring ref_formula = what[1].str();
        int col_formula=0, row_formula=0;
		utils::parsing_ref(ref_formula, col_formula, row_formula);col_formula--;//инче отсчет с 1
	
		ref_formula = utils::getColAddress(col_formula) + std::to_wstring(row_formula +current_table_row_ - tmp_row_);


		return ref_formula;
	}
    else if (what[2].matched)
        return what[2].str();    
    else if (what[3].matched)
        return what[3].str();
	else
		return L"";
}
std::wstring ods_table_state::replace_cell_column(boost::wsmatch const & what)
{
    if (what[1].matched)
	{
		std::wstring ref_formula = what[1].str();
        int col_formula=0, row_formula=0;
		utils::parsing_ref(ref_formula, col_formula, row_formula);col_formula--;
	
		ref_formula = utils::getColAddress(col_formula+current_table_column_ -tmp_column_)+boost::lexical_cast<std::wstring>(row_formula);

		return ref_formula;
	}
    else if (what[2].matched)
        return what[2].str();    
    else if (what[3].matched)
        return what[3].str();
	else
		return L"";
}

void ods_table_state::add_or_find_cell_shared_formula(std::wstring & formula, std::wstring ref, int ind)
{
	if (ind < 0)return;
	
	table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
	if (cell == NULL)return;	

	std::wstring odf_formula;
	
	if (false == formula.empty())
	{
		odf_formula = formulas_converter_table.convert_formula(formula);

		int moving_type = 0;
		
		std::vector<std::wstring> distance;
		boost::algorithm::split(distance, ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);
		if (distance.size() > 1)
		{
            int col1, row1, col2, row2;
			utils::parsing_ref(distance[0], col1, row1);
			utils::parsing_ref(distance[1], col2, row2);

			if (row2 - row1 > 0) moving_type = 2;
			if (col2 - col1 > 0)moving_type = 1;
		}
		ods_shared_formula_state state = {(unsigned int)ind, odf_formula,ref, current_table_column_,current_table_row_, moving_type};
		shared_formulas_.insert(std::make_pair((unsigned int)ind, state));
		
		cell->attlist_.table_formula_ = odf_formula;
		cells_.back().empty = false;
	}
	else
	{
		std::map<unsigned int, ods_shared_formula_state>::iterator pFind = shared_formulas_.find(ind);
		
		if (pFind != shared_formulas_.end())
		{
			odf_formula = pFind->second.formula;

			//поменять по ref формулу !!!
			if (pFind->second.moving_type == 1)
			{
				tmp_column_ = pFind->second.base_column;
				tmp_row_	= pFind->second.base_row;
				
				const std::wstring res = boost::regex_replace(
					odf_formula,
					boost::wregex(L"([a-zA-Z]{1,3}[0-9]{1,3})|(?='.*?')|(?=\".*?\")"),
					&ods_table_state::replace_cell_column,
					boost::match_default | boost::format_all);
				odf_formula = res;
			}
			else if (pFind->second.moving_type == 2)
			{
				tmp_column_ = pFind->second.base_column;
				tmp_row_	= pFind->second.base_row;
				
				const std::wstring res = boost::regex_replace(
					odf_formula,
					boost::wregex(L"([a-zA-Z]{1,3}[0-9]{1,3})|(?='.*?')|(?=\".*?\")"),
					&ods_table_state::replace_cell_row,
					boost::match_default | boost::format_all);
				odf_formula = res;
			}
			cell->attlist_.table_formula_ = odf_formula;				
			cells_.back().empty = false;
		}
	}
}
void ods_table_state::set_cell_array_formula(std::wstring & formula, std::wstring ref)
{
	set_cell_formula(formula);

	//; ??? C2:D5 или D1;F1;G; ... ???

 	std::vector<std::wstring> ref_cells;
	boost::algorithm::split(ref_cells,ref, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

    int row_span = 0;
    int col_span = 0;

	if (ref_cells.size() ==2)
	{
        int col1 = -1, row1 = -1;
        int col2 = -1, row2 = -1;

		utils::parsing_ref (ref_cells[0], col1, row1);
		utils::parsing_ref (ref_cells[1], col2, row2);

		row_span = abs(row2-row1) +1;
		col_span = abs(col2-col1) +1;

	}
	else if (ref_cells.size() ==1)	row_span = col_span = 1;
	else
	{
		row_span = col_span = 1;//???
	}

	if (col_span > 0 && row_span > 0)
	{
		table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
		if (cell == NULL)return;

		cell->attlist_extra_.table_number_matrix_columns_spanned_ = col_span;
		cell->attlist_extra_.table_number_matrix_rows_spanned_ = row_span;

	}
}

void ods_table_state::add_child_element( const office_element_ptr & child_element)
{
	office_table_->add_child_element(child_element);
}

void ods_table_state::convert_position(oox_table_position & oox_pos, double & x, double & y)//c 0 отсчет
{
	double sz_col=0;
    size_t curr_col = 0, i;
	
	for (i = 0; i < columns_.size(); i++)
	{
		if (oox_pos.col >(int)(columns_[i].repeated +  curr_col))
		{
			sz_col += (columns_[i].repeated ) * columns_[i].size;
		}
		else
		{
			sz_col += (oox_pos.col - curr_col ) * columns_[i].size;
			curr_col += (oox_pos.col - curr_col );
			break;
		}
		curr_col += columns_[i].repeated;
	}
	
	if ((int)curr_col  < oox_pos.col && false == columns_.empty())
	{
		sz_col += (oox_pos.col - curr_col) * columns_[columns_.size() - 1].size;
	}

	x = sz_col + oox_pos.col_off;

	double sz_row=0;
   
	size_t curr_row =0 ;
	for (i = 0; i < rows_.size(); i++)
	{
		if (oox_pos.row >(int)(rows_[i].repeated + curr_row))
		{
			sz_row += (rows_[i].repeated ) * rows_[i].size;
		}
		else
		{
			sz_row += (oox_pos.row - curr_row) * rows_[i].size;
		
			curr_row += (oox_pos.row - curr_row);
			break;
		}
		curr_row += rows_[i].repeated;
	}

	if ((int)curr_row < oox_pos.row && false == rows_.empty())
	{
		sz_row += (oox_pos.row - curr_row ) * rows_[rows_.size() - 1].size;
	}

	y = sz_row + oox_pos.row_off;
}


void ods_table_state::set_cell_text(odf_text_context* text_context, bool cash_value)
{
	if (text_context == NULL)return;

	for (size_t i = 0; i< text_context->text_elements_list_.size(); i++)
	{
		if (text_context->text_elements_list_[i].level ==0)
		{
			cells_.back().elm->add_child_element(text_context->text_elements_list_[i].elm);
		}
	}
	if (cash_value == false)
	{
		_CP_OPT(office_value_type) cell_type = office_value_type(office_value_type::String);

		table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
		if (cell)
		{
			if (!cell->attlist_.common_value_and_type_attlist_)
			{
				cell->attlist_.common_value_and_type_attlist_ = common_value_and_type_attlist();
			}
			cell->attlist_.common_value_and_type_attlist_->office_value_type_ = cell_type;
		}
		cells_.back().empty = false;

	}

	style* style_ = dynamic_cast<style*>(cells_.back().style_elm.get());
	if (!style_)return;	
	
	odf_writer::style_table_cell_properties	* table_cell_properties = style_->content_.get_style_table_cell_properties();

	//if (table_cell_properties && cash_value == false)
	//{
	//	table_cell_properties->content_.style_text_align_source_ = odf_writer::text_align_source(odf_writer::text_align_source::Fix);
	//}	
}
void ods_table_state::set_cell_value(const std::wstring & value, bool need_cash)
{
	if (cells_.empty())return;

	table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
	if (cell == NULL)return;

	if (!cell->attlist_.common_value_and_type_attlist_)
	{
		cell->attlist_.common_value_and_type_attlist_ = common_value_and_type_attlist();
		cell->attlist_.common_value_and_type_attlist_->office_value_type_ = office_value_type(office_value_type::Float);
		//временно... пока нет определялки типов
	}
	cells_.back().empty = false;
	
	if (cell->attlist_.common_value_and_type_attlist_->office_value_type_)
	{
		office_value_type::type type = cell->attlist_.common_value_and_type_attlist_->office_value_type_->get_type();
		switch(type)
		{
		case office_value_type::String:
			cell->attlist_.common_value_and_type_attlist_->office_string_value_ = value;
			break;
		case office_value_type::Boolean:
			cell->attlist_.common_value_and_type_attlist_->office_boolean_value_ = value;
			break;
		case office_value_type::Date:
			cell->attlist_.common_value_and_type_attlist_->office_date_value_ = utils::convert_date(value);
			break;
		case office_value_type::Time:
			cell->attlist_.common_value_and_type_attlist_->office_time_value_ = utils::convert_time(value);
			break;
		case office_value_type::DateTime:
		{
			std::wstring sVal = utils::convert_date_time(value, type);
			
			if (type == office_value_type::Date)
				cell->attlist_.common_value_and_type_attlist_->office_date_value_ = sVal;
			else
				cell->attlist_.common_value_and_type_attlist_->office_time_value_ = sVal;
			cell->attlist_.common_value_and_type_attlist_->office_value_type_ = office_value_type(type);
		}break;
		case office_value_type::Currency:
		case office_value_type::Percentage:
		case office_value_type::Float:
		case office_value_type::Scientific:
		case office_value_type::Fraction:
		default:
			cell->attlist_.common_value_and_type_attlist_->office_value_ = value;
		}
	}
	else
	{
		//general !!
	}
	
	//кэшированные значения
	if (false == value.empty())
	{
		if (is_cell_hyperlink())
		{
			need_cash = true;
			cell->attlist_.common_value_and_type_attlist_->office_string_value_ = boost::none;
		}

		bool need_test_cach = false;

		if (cell->attlist_.common_value_and_type_attlist_->office_value_type_)
		{
			if (cell->attlist_.common_value_and_type_attlist_->office_value_type_->get_type() == office_value_type::Float ||
 				cell->attlist_.common_value_and_type_attlist_->office_value_type_->get_type() == office_value_type::Currency ||  
 				cell->attlist_.common_value_and_type_attlist_->office_value_type_->get_type() == office_value_type::Scientific ||  
 				cell->attlist_.common_value_and_type_attlist_->office_value_type_->get_type() == office_value_type::Fraction ||  
				cell->attlist_.common_value_and_type_attlist_->office_value_type_->get_type() == office_value_type::Percentage)
					need_test_cach = true;
		}
		try
		{
			if (need_test_cach)
			{
				double test = boost::lexical_cast<double>(value);			
				need_cash = true;
			}
		}
		catch(...)
		{
			if (need_cash)
			{
				cell->attlist_.common_value_and_type_attlist_->office_value_ = boost::none;
				cell->attlist_.common_value_and_type_attlist_->office_value_type_ = office_value_type(office_value_type::String);
			}
		}

		if (need_cash)
		{
			ods_conversion_context* ods_context = dynamic_cast<ods_conversion_context*>(context_);

			if (ods_context)
			{
				ods_context->start_cell_text();
					ods_context->text_context()->add_text_content(value);
				ods_context->end_cell_text();
					
				set_cell_text( ods_context->text_context(), true);
			}
			else
			{
				context_->start_text_context();
				
				start_cell_text();
					context_->text_context()->add_text_content(value);
				end_cell_text();
					
				set_cell_text( context_->text_context(), true);
				
				context_->end_text_context();
			}
		}
	}
}
void ods_table_state::start_cell_text()
{
////////////
	office_element_ptr paragr_elm;
	create_element(L"text", L"p", paragr_elm, context_);
	
	context_->text_context()->start_paragraph(paragr_elm);

	if (is_cell_hyperlink())
	{
		ods_hyperlink_state & state = current_hyperlink();
		
		office_element_ptr text_a_elm;
		create_element(L"text", L"a", text_a_elm, context_);

		text_a* text_a_ = dynamic_cast<text_a*>(text_a_elm.get());
		if (text_a_ == NULL)return;

		text_a_->common_xlink_attlist_.type_ = xlink_type(xlink_type::Simple);
		text_a_->common_xlink_attlist_.href_ = state.link;
		
		context_->text_context()->start_element(text_a_elm); // может быть стоит сделать собственый???
		// libra дурит если в табличках будет вложенный span в гиперлинк ... оО (хотя это разрешено в спецификации!!!)

		context_->text_context()->single_paragraph_ = true;
	}
}

void ods_table_state::end_cell_text()
{
	if (context_->text_context())
	{
		if (is_cell_hyperlink())	context_->text_context()->end_element();
		
		context_->text_context()->end_paragraph();
	}
}
void ods_table_state::end_cell()
{
	if (cells_.empty())return;

    if (cells_.back().comment_idx >= 0)
	{
		office_element_ptr & comm_elm = comments_[cells_.back().comment_idx].elm;
		cells_.back().elm->add_child_element(comm_elm);
		comments_[cells_.back().comment_idx].used = true;
	}
    if (false == cells_.back().data_validation_name.empty())
	{
		table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
		if (cell)cell->attlist_.table_content_validation_name_ = cells_.back().data_validation_name;
	}
	if (cells_.back().empty)
	{
		table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
		if (cell)cell->attlist_.common_value_and_type_attlist_ = boost::none;
	}
	if (map_merged_cells.empty())
	{
		cells_.pop_back();
	}
}

void ods_table_state::add_default_cell( int repeated)
{
	if (repeated < 1) return;

    int comment_idx = is_cell_comment(current_table_column_ + 1, current_table_row_, repeated);
	if (comment_idx  >= 0 && repeated > 1)
	{
		//делим на 3 - до, с комметом, после;
        int c = current_table_column_;

		add_default_cell(comments_[comment_idx].col - c - 1);
		add_default_cell(1);
		add_default_cell(repeated + c - comments_[comment_idx].col);

		return;
	}
//-----------------------------------------------------------------------------------------
	std::map<int, std::map<int, _spanned_info>>::iterator pFindRow = map_merged_cells.find(current_table_row_);

	bool bSpanned = false;
	if (pFindRow != map_merged_cells.end())
	{	
		for (std::map<int, _spanned_info>::iterator it = pFindRow->second.begin(); !bSpanned && it != pFindRow->second.end(); ++it)
		{
			if (it->first < current_table_column_ + repeated + 1 && it->first >= current_table_column_ + 1)
			{
				if (repeated > 1)
				{
					//делим на 3 - до, с spanned, после;
					int c = current_table_column_;

					add_default_cell(it->first - c - 1);
					add_default_cell(1);
					add_default_cell(repeated + c - current_table_column_);

					return;
				}
				else
				{
					bSpanned = true;
					break;
				}
			}
		}
	}
//-----------------------------------------------------------------------------------------
	data_validation_state::_ref ref;
	std::wstring validation_name = is_cell_data_validation(current_table_column_ + 1, current_table_row_, repeated, ref);

	int repeated_validation = (std::min)(ref.col_end, current_table_column_ + (int)repeated) - (std::max)(ref.col_start, current_table_column_ + 1) + 1;

	if (false == validation_name.empty() && repeated > 1 && repeated_validation != repeated)
	{
		//делим на 3 - до, с validation, после;
		int c = current_table_column_;

		add_default_cell(ref.col_start - c - 1);
		add_default_cell(repeated_validation);
		add_default_cell(repeated + c - current_table_column_);

		return;
	}
//-----------------------------------------------------------------------------------------
	bool bCovered = false;
	if (!bSpanned)
	{
		for (size_t i = 0; i < current_covered_rows_.size(); i++)
		{
			if (current_covered_rows_[i].start_col <= current_table_column_ && current_table_column_ + repeated <= current_covered_rows_[i].start_col + current_covered_rows_[i].count_cols)
			{
				current_covered_cols_ = current_covered_rows_[i].start_col + current_covered_rows_[i].count_cols - current_table_column_;
				bCovered = true;
				break;
			}
			else if (!(	current_table_column_ + repeated < current_covered_rows_[i].start_col || 
						current_table_column_ > current_covered_rows_[i].start_col + current_covered_rows_[i].count_cols))
			{
				int c = current_table_column_;
				int split = current_covered_rows_[i].start_col > current_table_column_ ? current_covered_rows_[i].start_col : 
					current_covered_rows_[i].start_col + current_covered_rows_[i].count_cols;

				if (split != current_table_column_ && split != current_table_column_ + repeated)
				{
					add_default_cell(split - c);
					add_default_cell(repeated + c - split);
					return;
				}
			}
		}		
	}
//-----------------------------------------------------------------------------------------
	office_element_ptr default_cell_elm;
	if (bCovered)
	{
		create_element(L"table", L"covered-table-cell", default_cell_elm, context_);
	}
	else
	{
		create_element(L"table", L"table-cell", default_cell_elm, context_);
	}

	current_row_element()->add_child_element(default_cell_elm);
	
	table_table_cell* cell = dynamic_cast<table_table_cell*>(default_cell_elm.get());
	table_covered_table_cell* covered_cell = dynamic_cast<table_covered_table_cell*>(default_cell_elm.get());

	if (bSpanned)
	{
		int spanned_rows = 0, spanned_cols = 0;
		if (cell && isSpannedCell(current_table_column_, current_table_row_, spanned_cols, spanned_rows))
		{
			cell->attlist_extra_.table_number_columns_spanned_ = spanned_cols;
			cell->attlist_extra_.table_number_rows_spanned_ = spanned_rows;
		}
	}
	
 	ods_cell_state state;
	
	state.empty = true;
	state.elm = default_cell_elm;  
	state.repeated = repeated; 
    state.row = current_table_row_;  
	state.col = current_table_column_ + 1;

	state.hyperlink_idx			= is_cell_hyperlink(state.col, current_table_row_);
	state.data_validation_name	= validation_name;
	state.comment_idx			= comment_idx;
	
	cells_.push_back(state);
	
	current_table_column_ += state.repeated;

	if (cell)
	{
		cell->attlist_.table_number_columns_repeated_ = repeated;

		if (!row_default_cell_style_name_.empty())
			cell->attlist_.table_style_name_ = row_default_cell_style_name_;
	}
	if (covered_cell)
	{
		covered_cell->attlist_.table_number_columns_repeated_ = repeated;

		if (!row_default_cell_style_name_.empty())
			covered_cell->attlist_.table_style_name_ = row_default_cell_style_name_;

		current_covered_cols_ -= repeated;
	}
	end_cell();
}
///////////////////////////////////////////////////
void ods_table_state::start_pilot_table(office_element_ptr & elm)
{
	pilot_table_state_.elm = elm;
}
void ods_table_state::end_pilot_table()
{
}
void ods_table_state::start_conditional_formats()
{
	office_element_ptr elm;
	create_element(L"calcext", L"conditional-formats", elm, context_);

	current_level_.back()->add_child_element(elm);
	current_level_.push_back(elm);

}
void ods_table_state::end_conditional_formats()
{
	current_level_.pop_back();
}
void ods_table_state::start_conditional_format(std::wstring ref)
{
	office_element_ptr elm;
	create_element(L"calcext", L"conditional-format", elm, context_);

	current_level_.back()->add_child_element(elm);
	current_level_.push_back(elm);

	calcext_conditional_format* cond_format = dynamic_cast<calcext_conditional_format*>(elm.get());

	if (cond_format)
	{
		formulasconvert::oox2odf_converter converter;
		std::wstring out = converter.convert_ref(ref);

		cond_format->calcext_target_range_address_ = out;
		//проверить конвертацию на диапазонах с именами листов в кавычках и с пробелами
	}
}
void ods_table_state::end_conditional_format()
{
	current_level_.pop_back();
}
void ods_table_state::start_conditional_rule(int rule_type, _CP_OPT(unsigned int) rank, _CP_OPT(bool) bottom, _CP_OPT(bool) percent)
{
	office_element_ptr elm;

		 if (rule_type == 3) create_element(L"calcext", L"color-scale", elm, context_); 
	else if (rule_type == 7) create_element(L"calcext", L"data-bar", elm ,context_);
	else if (rule_type == 10)create_element(L"calcext", L"icon-set", elm, context_);
	else if (rule_type == 14)create_element(L"calcext", L"date-is", elm, context_);
	else
	{
		create_element(L"calcext", L"condition", elm, context_);
		calcext_condition* condition = dynamic_cast<calcext_condition*>	 (elm.get());
		
		if (condition) 
		{
			calcext_conditional_format* condition_fmt	= dynamic_cast<calcext_conditional_format*>	 (current_level_.back().get());
			if ((condition_fmt) && (condition_fmt->calcext_target_range_address_))
			{
				std::wstring cell, table,test = *condition_fmt->calcext_target_range_address_;
				std::vector< std::wstring > splitted;
			
				boost::algorithm::split(splitted, test, boost::algorithm::is_any_of(L"!"), boost::algorithm::token_compress_on);
    			if (splitted.size()>1)
				{
					table = splitted[0];
					test = splitted[1];
				}else table = office_table_name_;
				boost::algorithm::split(splitted, test, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);
				cell = splitted[0];

				std::wstring col, row;

				utils::splitCellAddress(cell, col, row);
				if (col.empty()) col = L".A";
				if (row.empty()) row = L"1";

				if (std::wstring::npos != table.find(L" "))
				{
					if (table[0] != L'\'')
					{
						table = L"'" + table + L"'";
					}
				}
				condition->attr_.calcext_base_cell_address_ = table + col + row;
			}
			switch(rule_type)
			{
				case 0:	condition->attr_.calcext_value_		= L"above-average";		break;
				case 1:	condition->attr_.calcext_value_		= L"begins-with()";		break;
				case 4: condition->attr_.calcext_value_		= L"contains-text()";	break;
				case 5: condition->attr_.calcext_value_		= L"is-error";			break;
				case 6: condition->attr_.calcext_value_		= L"contains-text()";	break;
				case 8: condition->attr_.calcext_value_		= L"duplicate";			break;
				case 9: condition->attr_.calcext_value_		= L"formula-is()";		break;
				case 11: condition->attr_.calcext_value_	= L"not-contains-text()"; break;
				case 12: condition->attr_.calcext_value_	= L"is-no-error";		break;
				case 13: condition->attr_.calcext_value_	= L"not-contains-text()"; break;
				case 15:
				{
					if ((bottom) && (*bottom)) 	condition->attr_.calcext_value_ = L"bottom";
					else 						condition->attr_.calcext_value_ = L"top";
					
					if (percent && (*percent))	*condition->attr_.calcext_value_ += L"-percent(";
					else						*condition->attr_.calcext_value_ += L"-elements(";

					*condition->attr_.calcext_value_ += std::to_wstring(rank.get_value_or(10)) + L")";
				}break;
				case 16: condition->attr_.calcext_value_ = L"unique"; break;
				case 17:	condition->attr_.calcext_value_ = L"ends-with()";break;
				case 2: /*cellIs*/
				default: break;
			}
		}
	}

	current_level_.back()->add_child_element(elm);
	current_level_.push_back(elm);
}

void ods_table_state::end_conditional_rule()
{
	current_level_.pop_back();
}

void ods_table_state::set_conditional_formula(std::wstring formula)
{
	calcext_condition* condition = dynamic_cast<calcext_condition*>	 (current_level_.back().get());

	if (!condition) return;

	std::wstring odfFormula = formulas_converter_table.convert_conditional_formula(formula);
		
	std::wstring operator_;
	bool s = false;
	bool split = false;

	operator_ = condition->attr_.calcext_value_.get_value_or(L"");

	size_t f_start = operator_.find(L"("); 
	size_t f_end = operator_.rfind(L")"); 
	if (f_start != std::wstring::npos && f_end != std::wstring::npos) 
	{
		if (f_start < f_end - 1) split = true;
		s = true; 
		operator_ = operator_.substr(0, f_end);
	}
	if (operator_.empty())
		operator_ = odfFormula;
	else
		operator_ += (split ? L"," : L" ") + odfFormula + (s ? L")" : L"");

	if ((std::wstring::npos == operator_.find(L"contains-text") &&
		std::wstring::npos == operator_.find(L"begins-with") && 
		std::wstring::npos == operator_.find(L"ends-with"))
		|| !split )
		condition->attr_.calcext_value_= operator_;
}
void ods_table_state::set_conditional_style_name(const std::wstring &style_name)
{
	calcext_condition*	condition	 = dynamic_cast<calcext_condition*>	 (current_level_.back().get());
	calcext_date_is*	date_is		 = dynamic_cast<calcext_date_is*>	 (current_level_.back().get());

	if (condition)	condition->attr_.calcext_apply_style_name_ = style_name;
	if (date_is)	date_is->attr_.calcext_style_ = style_name;
}
void ods_table_state::set_conditional_time(int period)
{
	calcext_date_is* date_is = dynamic_cast<calcext_date_is*>(current_level_.back().get());
	if (date_is)
	{
		switch (period)
		{
		case 1: date_is->attr_.calcext_date_ = odf_types::time_period::yesterday; break;
		case 2: date_is->attr_.calcext_date_ = odf_types::time_period::tomorrow; break;
		case 3: date_is->attr_.calcext_date_ = odf_types::time_period::last7Days; break;
		case 4: date_is->attr_.calcext_date_ = odf_types::time_period::thisMonth; break;
		case 5: date_is->attr_.calcext_date_ = odf_types::time_period::lastMonth; break;
		case 6: date_is->attr_.calcext_date_ = odf_types::time_period::nextMonth; break;
		case 7: date_is->attr_.calcext_date_ = odf_types::time_period::thisWeek; break;
		case 8: date_is->attr_.calcext_date_ = odf_types::time_period::lastWeek; break;
		case 9: date_is->attr_.calcext_date_ = odf_types::time_period::nextWeek; break;
		case 0:
		default:
			date_is->attr_.calcext_date_ = odf_types::time_period::today;
		}
	}
}
void ods_table_state::set_conditional_text(const std::wstring &text)
{
	if (text.empty()) return;

	calcext_condition* condition = dynamic_cast<calcext_condition*>(current_level_.back().get());
	if (!condition) return;

	if ((condition->attr_.calcext_value_) && 
		(std::wstring::npos != condition->attr_.calcext_value_->find(L"contains-text") || 
		 std::wstring::npos != condition->attr_.calcext_value_->find(L"ends-with") ||
		 std::wstring::npos != condition->attr_.calcext_value_->find(L"begins-with")))
	{
		std::wstring operator_;
		bool s = false;
		bool split = false;

		operator_ = *condition->attr_.calcext_value_;
		size_t f_start = operator_.find(L"("); 
		size_t f_end = operator_.rfind(L")"); 
		if (f_start != std::wstring::npos && f_end != std::wstring::npos) 
		{
			if (f_start < f_end - 1) split = true;
			s = true; 
			operator_ = operator_.substr(0, f_end);
		}		
		operator_ += (split ? L"," : L"") + std::wstring(L"\"") + text + L"\"" + (s ? L")" : L"");
		condition->attr_.calcext_value_= operator_;
	}
}
void ods_table_state::set_conditional_operator(int _operator)
{
	calcext_condition*	condition	 = dynamic_cast<calcext_condition*>	 (current_level_.back().get());
	if (condition)
	{
		switch(_operator)
		{
		case 0:	condition->attr_.calcext_value_ = L"begins-with()";		break;
		case 1:	condition->attr_.calcext_value_ = L"between()";			break;
		case 2:	condition->attr_.calcext_value_ = L"contains-text()";	break;
		case 3:	condition->attr_.calcext_value_ = L"ends-with()";		break;
		case 4:	condition->attr_.calcext_value_ = L"=";					break;
		case 5:	condition->attr_.calcext_value_ = L">";					break;
		case 6:	condition->attr_.calcext_value_ = L">=";				break;
		case 7:	condition->attr_.calcext_value_ = L"<";					break;
		case 8:	condition->attr_.calcext_value_ = L"<=";				break;
		case 9:	condition->attr_.calcext_value_ = L"not-between()";		break;
		case 10:condition->attr_.calcext_value_ = L"not-contains-text()"; break;
		case 11:condition->attr_.calcext_value_ = L"!=";				break;
		}
	}
}
void ods_table_state::set_conditional_value(int type, std::wstring value )
{
	calcext_icon_set* icon_set		 = dynamic_cast<calcext_icon_set*>	 (current_level_.back().get());
	calcext_data_bar* data_bar		 = dynamic_cast<calcext_data_bar*>	 (current_level_.back().get());
	calcext_color_scale* color_scale = dynamic_cast<calcext_color_scale*>(current_level_.back().get());

	if (icon_set || data_bar)
	{
		office_element_ptr elm;
		create_element(L"calcext", L"formatting-entry", elm, context_);
		
		current_level_.back()->add_child_element(elm);

		calcext_formatting_entry * entry = dynamic_cast<calcext_formatting_entry*>(elm.get());
		if (entry)
		{
			switch(type)
			{
				case 1: entry->calcext_type_ = calcext_type(calcext_type::Maximum); break;
				case 2: entry->calcext_type_ = calcext_type(calcext_type::Minimum); break;
				case 4: entry->calcext_type_ = calcext_type(calcext_type::Percent); break;
				case 5: entry->calcext_type_ = calcext_type(calcext_type::Percentile); break;
				case 6: entry->calcext_type_ = calcext_type(calcext_type::AutoMinimum); break;
				case 7: entry->calcext_type_ = calcext_type(calcext_type::AutoMaximum); break;
				case 0: //Formula	
				case 3: //Number
				default: entry->calcext_type_ = calcext_type(calcext_type::Number);
			}
			entry->calcext_value_ = value;
		}
	}

	if (color_scale)
	{
		office_element_ptr elm;
		create_element(L"calcext", L"color-scale-entry", elm, context_);
		
		current_level_.back()->add_child_element(elm);

		calcext_color_scale_entry * entry = dynamic_cast<calcext_color_scale_entry*>(elm.get());
		if (entry)
		{
			switch(type)
			{				
				case 0: //Formula	
				case 1: entry->calcext_type_ = calcext_type(calcext_type::Maximum);		break;
				case 2: entry->calcext_type_ = calcext_type(calcext_type::Minimum);		break;
				case 4: entry->calcext_type_ = calcext_type(calcext_type::Percent);		break;
				case 5: entry->calcext_type_ = calcext_type(calcext_type::Percentile);	break;
				case 3: //Number
				default: entry->calcext_type_ = calcext_type(calcext_type::Number);
			}
			entry->calcext_value_ = value;
		}
		///color???? - прихоодят выше уровнем !!
	}

}
void ods_table_state::set_conditional_iconset(int type_iconset)
{
	calcext_icon_set* cond_format = dynamic_cast<calcext_icon_set*>(current_level_.back().get());

	if (cond_format)
	{
		cond_format->attr_.calcext_icon_set_type_ = iconset_type((iconset_type::type)type_iconset);
	}
}
void ods_table_state::add_conditional_colorscale(int index, _CP_OPT(color) color)
{
	calcext_color_scale *scale = dynamic_cast<calcext_color_scale*>(current_level_.back().get());

	if (!scale) return;
	if (index >= (int)scale->content_.size() || index < 0) return;

	calcext_color_scale_entry* color_scale_entry = dynamic_cast<calcext_color_scale_entry*>(scale->content_[index].get());

	if (color_scale_entry)
	{
		color_scale_entry->calcext_color_ = color;
	}
}
void ods_table_state::set_conditional_databar_color(_CP_OPT(color) color)
{
	calcext_data_bar* cond_format = dynamic_cast<calcext_data_bar*>(current_level_.back().get());

	if (cond_format)
	{
		cond_format->attr_.calcext_positive_color_ = color;
	}
}

}
}
