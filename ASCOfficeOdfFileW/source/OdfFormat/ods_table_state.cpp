#include "precompiled_cpodf.h"
#include "logging.h"

#include <cpdoccore/formulasconvert.h>

#include "ods_table_state.h"
#include "odf_text_context.h"
#include "ods_conversion_context.h"

#include "table.h"
#include "office_annotation.h"
#include "calcext_elements.h"
#include "styles.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_graphic_properties.h"

namespace cpdoccore {
namespace odf {

int ods_table_state::current_table_column_ = 0;
int ods_table_state::current_table_row_ = 0;

int ods_table_state::tmp_column_ =0;
int ods_table_state::tmp_row_ =0;

namespace utils//////////////////////////////////////////// ќЅўјя хрень .. вытащить что ли в utils ???

{
std::wstring convert_date(std::wstring & oox_date)
{
	int iDate = 0;

	try
	{
		iDate = boost::lexical_cast<int>(oox_date);
	}catch(...)
	{
		return oox_date;
	}

	boost::gregorian::date date_ = boost::gregorian::date(1900, 1, 1) + boost::gregorian::date_duration(iDate-2);

	//to for example, "1899-12-31T05:37:46.66569
	std::wstring date_str = boost::lexical_cast<std::wstring>(date_.year())
							+ L"-" +
							(date_.month() < 10 ? L"0": L"") + boost::lexical_cast<std::wstring>(date_.month()) 
							+ L"-" +
							(date_.day() < 10 ? L"0": L"") + boost::lexical_cast<std::wstring>(date_.day());
	return date_str;
}

std::wstring convert_time(std::wstring & oox_time)
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
	int hours=0, minutes=0;
	double sec=0;
	
	boost::posix_time::time_duration day(24, 0, 0);
	
	double millisec = day.total_milliseconds() * dTime;

	sec = millisec /1000.;
	hours = sec/60./60.;
	minutes = (sec - (hours * 60 * 60))/60.;
	sec = sec - (hours *60 + minutes) * 60.;

	int sec1 = sec;

	std::wstring time_str = std::wstring(L"PT") +
							(hours < 10 ? L"0" : L"") + boost::lexical_cast<std::wstring>(hours)
							+ std::wstring(L"H") +
							(minutes < 10 ? L"0" : L"") + boost::lexical_cast<std::wstring>(minutes) 
							+ std::wstring(L"M") +
							(sec1 < 10 ? L"0" : L"") + boost::lexical_cast<std::wstring>(sec1)
							+ std::wstring(L"S");

	return time_str;
}
};

///////////////////////////////////////////////////////////////

ods_table_state::ods_table_state(ods_conversion_context & Context, office_element_ptr & elm): context_(Context),drawing_context_(&Context)
{     
	office_table_ = elm; 

	current_table_row_ =0;
	current_table_column_ =0;

	current_level_.push_back(office_table_);

//default 	dimension

	dimension_columns = 1024;
	dimension_row = 1024;


	defaut_row_height_ = 9;
	defaut_column_width_ = 28.34467120181406 * 1.674;// 

}

void ods_table_state::set_table_name(std::wstring name)
{
	office_table_name_ = name;
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	table->table_table_attlist_.table_name_ = name;
}

void ods_table_state::set_table_hidden(bool Val)
{
	if (!office_table_style_)return;

	style_table_properties *table_properties = office_table_style_->style_content_.get_style_table_properties();
	if (table_properties == NULL)return;

	table_properties->table_format_properties_.table_display_ = !Val;

}

void ods_table_state::set_print_range(std::wstring range)
{
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	table->table_table_attlist_.table_print_ranges_ = range;
}

void ods_table_state::set_table_tab_color(_CP_OPT(color) & _color)
{
	if (!office_table_style_)return;

	style_table_properties *table_properties = office_table_style_->style_content_.get_style_table_properties();
	if (table_properties == NULL)return;

	table_properties->table_format_properties_.tableooo_tab_color_ = _color;
}

void ods_table_state::set_table_style(office_element_ptr & elm)
{	
	office_table_style_ = dynamic_cast<style*>(elm.get());

	if (!office_table_style_)return;

	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;
	
	table->table_table_attlist_.table_style_name_ = office_table_style_->style_name_;
	//потом в принципе и по имени можно будет св€зать(найти)


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

void ods_table_state::add_column(office_element_ptr & elm, int repeated,office_element_ptr & style_elm)
{
	current_level_.back()->add_child_element(elm);

	std::wstring style_name;

	odf::style* style = dynamic_cast<odf::style*>(style_elm.get());
	if (style)style_name = style->style_name_;

	ods_element_state state = {elm, repeated,style_name, style_elm, defaut_column_width_ , current_level_.size()};
  
	if (repeated > 10000)repeated = 1024;//????

	current_table_column_ += repeated;
    columns_.push_back(state);

	table_table_column* column = dynamic_cast<table_table_column*>(columns_.back().elm.get());
	if (column == NULL)return;

	if (style_name.length()>0) column->table_table_column_attlist_.table_style_name_ = style_ref(style_name);
	column->table_table_column_attlist_.table_number_columns_repeated_ = repeated;
	
}
void ods_table_state::set_column_default_cell_style(std::wstring & style_name)
{
	if (style_name.length() < 1)return;

	table_table_column* column = dynamic_cast<table_table_column*>(columns_.back().elm.get());
	if (column == NULL)return;

	column->table_table_column_attlist_.table_default_cell_style_name_ = style_ref(style_name);
}

void ods_table_state::set_column_width(double width)//pt
{
	odf::style* style = dynamic_cast<odf::style*>(columns_.back().style_elm.get());
	if (!style)return;		

	style_table_column_properties * column_properties = style->style_content_.get_style_table_column_properties();
 	if (column_properties == NULL)return; //error ????

	columns_.back().size = width; //pt

	column_properties->style_table_column_properties_attlist_.style_column_width_ = length(length(width,length::pt).get_value_unit(length::cm),length::cm);
}
void ods_table_state::set_column_optimal_width(bool val)
{
	odf::style* style = dynamic_cast<odf::style*>(columns_.back().style_elm.get());
	if (!style)return;		

	style_table_column_properties * column_properties = style->style_content_.get_style_table_column_properties();
 	if (column_properties == NULL)return; //error ????

	column_properties->style_table_column_properties_attlist_.style_use_optimal_column_width_ = val;

}

void ods_table_state::set_table_dimension(int col, int row)
{
	if (col<1 || row <1 )return;

	if (dimension_columns < col)	dimension_columns = col +1;
	if (dimension_row < row)		dimension_row = row+1;
}

void ods_table_state::add_row(office_element_ptr & elm, int repeated,office_element_ptr & style_elm)
{
    current_table_column_ = 0; 
    current_table_row_+=repeated;

	current_level_.back()->add_child_element(elm);

	std::wstring style_name;

	odf::style* style = dynamic_cast<odf::style*>(style_elm.get());
	if (style)style_name = style->style_name_;

	ods_element_state state = {elm, repeated,style_name, style_elm, defaut_row_height_ , current_level_.size()};
  
    rows_.push_back(state);

	table_table_row* row = dynamic_cast<table_table_row*>(rows_.back().elm.get());
	if (row == NULL)return;

	if (style_name.length()>0) row->table_table_row_attlist_.table_style_name_ = style_ref(style_name);
	row->table_table_row_attlist_.table_number_rows_repeated_ = repeated;

	row_default_cell_style_name_= L"";

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

	rows_.back().size = height;//pt

	row_properties->style_table_row_properties_attlist_.style_row_height_ = length(height,length::pt);//length(length(height,length::pt).get_value_unit(length::cm),length::cm);

}

bool ods_table_state::is_cell_hyperlink()
{
	if (cells_.size()<1)return false;
	return cells_.back().hyperlink_idx >=0 ? true : false;
}
bool ods_table_state::is_cell_comment()
{
	if (cells_.size()<1)return false;
	return cells_.back().comment_idx >=0 ? true : false;
}

int ods_table_state::is_cell_hyperlink(int col, int row)
{
	for (long i=0; i< hyperlinks_.size();i++)
	{
		if (hyperlinks_[i].col == col && hyperlinks_[i].row == row)
		{
			return  i;
		}
	}
	return -1;
}
int ods_table_state::is_cell_comment(int col, int row, int repeate_col)
{
	for (long i=0; i< comments_.size();i++)
	{
		if ((comments_[i].col < col+repeate_col && comments_[i].col >= col) && comments_[i].row == row)
		{
			return  i;
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

void ods_table_state::set_row_default_cell_style(std::wstring & style_name)
{
	row_default_cell_style_name_= style_name;	//об€зательно нужно определить default-style (table_cell)!!!
	
	//if (style_name.length() < 1) return;

	//table_table_row* row = dynamic_cast<table_table_row*>(rows_.back().elm.get());
	//if (row == NULL)return;

	//row->table_table_row_attlist_.table_default_cell_style_name_ = style_ref(style_name);
}

office_element_ptr  & ods_table_state::current_row_element()
{
	if (rows_.size()>0)
		return rows_.back().elm;
	else
		throw;
}
office_element_ptr  & ods_table_state::current_cell_element()
{
	if (cells_.size()>0)
		return cells_.back().elm;
	else
		throw;
}
ods_hyperlink_state & ods_table_state::current_hyperlink()
{
	if ((cells_.size()>0 && hyperlinks_.size()>0) && (cells_.back().hyperlink_idx>=0) )
	{
		return hyperlinks_[cells_.back().hyperlink_idx];
	}
	else
		throw;
}

void ods_table_state::start_cell(office_element_ptr & elm, office_element_ptr & style_elm)
{
	current_row_element()->add_child_element(elm);
	
	table_table_cell* cell = dynamic_cast<table_table_cell*>(elm.get());
	if (cell == NULL)return;
	
	std::wstring style_name;

	odf::style* style = dynamic_cast<odf::style*>(style_elm.get());
	if (style)style_name = style->style_name_;
	else style_name = row_default_cell_style_name_;

	if (style_name.length() > 0) 
		cell->table_table_cell_attlist_.table_style_name_=	style_name;

	ods_cell_state state;
	
	state.empty = true;
	state.elm = elm;  state.repeated = 1;  state.style_name = style_name; state.style_elm = style_elm;
	state.row=current_table_row_;  state.col =current_table_column_+1; 

	state.hyperlink_idx = is_cell_hyperlink(state.col, state.row);
	state.comment_idx = is_cell_comment(state.col, state.row);

	current_table_column_ +=  state.repeated;  
    cells_.push_back(state);
}

void ods_table_state::set_cell_format_value(office_value_type::type value_type)
{
	if (cells_.size() < 1)return;
	if (value_type == office_value_type::Custom)return;	//general .. need detect

	table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
	if (cell == NULL)return;

	common_value_and_type_attlist cell_type;
	cell_type.office_value_type_ = office_value_type(value_type);

	cell->table_table_cell_attlist_.common_value_and_type_attlist_ = cell_type;

}
void ods_table_state::set_cell_type(int type)
{
	if (cells_.size() < 1)return;

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
	case 6:// обычна€
		cell_type = office_value_type(office_value_type::String);
		break;
	}
	if (cell_type)
	{
		if (!cell->table_table_cell_attlist_.common_value_and_type_attlist_)
		{
			cell->table_table_cell_attlist_.common_value_and_type_attlist_ = common_value_and_type_attlist();
		}
		cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_value_type_ = cell_type;
	}
	else
	{
		//сходить по стилю €чейки - проверить тип €чейки и формат - numFmt
	}
}
void ods_table_state::add_definded_expression(office_element_ptr & elm)
{
	if (!table_defined_expressions_)
	{
		create_element(L"table", L"named-expressions",table_defined_expressions_,&context_);
		office_table_->add_child_element(table_defined_expressions_);
	}
	if (!table_defined_expressions_)return;
	table_defined_expressions_->add_child_element(elm);
}
void ods_table_state::add_hyperlink(std::wstring & ref,int col, int row, std::wstring & link)
{
	ods_hyperlink_state state;
	

	state.row=row;  state.col =col; state.ref = ref, state.link = link;

	hyperlinks_.push_back(state);
}
void ods_table_state::start_comment(int col, int row, std::wstring & author)
{
	ods_comment_state state;

	state.row=row;  state.col =col; state.author = author;	
	create_element(L"office", L"annotation",state.elm,&context_);

	comments_.push_back(state);
}

void ods_table_state::set_comment_rect(double l, double t, double w, double h)
{
	if (comments_.size() < 1)return;

	office_annotation * annotation = dynamic_cast<office_annotation*>(comments_.back().elm.get());
	if (!annotation)return;		

	annotation->office_annotation_attr_.svg_y_		= length(t/10.,length::cm);
	annotation->office_annotation_attr_.svg_x_		= length(l/10.,length::cm);
	annotation->office_annotation_attr_.svg_width_	= length(w/10.,length::cm);
	annotation->office_annotation_attr_.svg_height_	= length(h/10.,length::cm);
}
void ods_table_state::end_comment(odf_text_context *text_context)
{
	if (text_context == NULL || comments_.size() <1 )return;

	for (long i=0; i< text_context->text_elements_list_.size(); i++)
	{
		if (text_context->text_elements_list_[i].level ==0 && comments_.back().elm)
		{
			comments_.back().elm->add_child_element(text_context->text_elements_list_[i].elm);
		}
	}
	if (comments_.back().author.length() > 0 && comments_.back().elm)
	{
		office_element_ptr dc_elm;
		create_element(L"dc", L"creator",dc_elm,&context_);
		if (dc_elm)
		{
			dc_elm->add_text(comments_.back().author);
			comments_.back().elm->add_child_element(dc_elm);
		}
	}
}

void ods_table_state::set_merge_cells(int start_col, int start_row, int end_col, int end_row)
{
	//потом можно переделать (оптимизировать) - добавл€ть мержи при добавлении €чеек
	//вс€ко выгоднее хранить данные о мержах, а не шерстить каждый раз ¬—≈ €чейки дл€ добавлени€ фенечки
	//todooo
	//разобратьс€ когда нужно писать covered_cell 
	if (end_col - start_col < 0)return;
	if (end_row - start_row < 0)return;

	int spanned_cols = end_col - start_col + 1;
	int spanned_rows = end_row - start_row + 1;

	if (spanned_cols > 10000)spanned_cols = 1024;

	for (int i = 0; i < cells_.size(); i++)
	{
		if (cells_[i].row > end_row) break;

		if (/*cells_[i].row <= end_row && */cells_[i].row >= start_row)
		{
			if (/*(cells_[i].col <= end_col || cells_[i].col + cells_[i].repeated-1 <= end_col) 
				&&*/ (cells_[i].col >= start_col/* || cells_[i].col + cells_[i].repeated-1 >= start_col*/))
			{
				table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_[i].elm.get());
				if (cell == NULL)return;

				cell->table_table_cell_attlist_extra_.table_number_columns_spanned_ = spanned_cols;
				cell->table_table_cell_attlist_extra_.table_number_rows_spanned_ = spanned_rows;

				break;
			}
		}
	}
}
static formulasconvert::oox2odf_converter formulas_converter;

void ods_table_state::set_cell_formula(std::wstring & formula)
{
	std::wstring odfFormula = formulas_converter.convert_formula(formula);
	
	table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
	if (cell == NULL)return;

	cell->table_table_cell_attlist_.table_formula_ = odfFormula;
	cells_.back().empty = false;
}

std::wstring ods_table_state::replace_cell_row(boost::wsmatch const & what)
{
    if (what[1].matched)
	{
		std::wstring ref_formula = what[1].str();
		int col_formula=0, row_formula=0;
		utils::parsing_ref(ref_formula, col_formula, row_formula);col_formula--;//инче отсчет с 1
	
		ref_formula = utils::getColAddress(col_formula)+boost::lexical_cast<std::wstring>(row_formula+current_table_row_ -tmp_row_);


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
	
	std::wstring odf_formula;
	
	if (formula.size() > 0)
	{
		odf_formula = formulas_converter.convert_formula(formula);

		int moving_type = 0;
		
		std::vector<std::wstring> distance;
		boost::algorithm::split(distance, ref,boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);
		if (distance.size() >1)
		{
			int col1, row1, col2, row2;
			utils::parsing_ref(distance[0],col1,row1);
			utils::parsing_ref(distance[1],col2,row2);

			if (row2-row1 >0)moving_type = 2;
			if (col2-col1 >0)moving_type = 1;
		}
		ods_shared_formula_state state = {ind, odf_formula,ref, current_table_column_,current_table_row_, moving_type};
		shared_formulas_.push_back(state);
	}
	else
	{
		for (long i=0; i<shared_formulas_.size() ;i++)
		{
			if (shared_formulas_[i].index == ind)
			{
				odf_formula = shared_formulas_[i].formula;
				table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
				if (cell == NULL)return;
				//помен€ть по ref формулу !!!
				if (shared_formulas_[i].moving_type == 1)
				{
					tmp_column_ = shared_formulas_[i].base_column;
					tmp_row_	= shared_formulas_[i].base_row;
					
					const std::wstring res = boost::regex_replace(
						odf_formula,
						boost::wregex(L"([a-zA-Z]{1,3}[0-9]{1,3})|(?='.*?')|(?=\".*?\")"),
						&ods_table_state::replace_cell_column,
						boost::match_default | boost::format_all);
					odf_formula = res;
				}
				if (shared_formulas_[i].moving_type == 2)
				{
					tmp_column_ = shared_formulas_[i].base_column;
					tmp_row_	= shared_formulas_[i].base_row;
					
					const std::wstring res = boost::regex_replace(
						odf_formula,
						boost::wregex(L"([a-zA-Z]{1,3}[0-9]{1,3})|(?='.*?')|(?=\".*?\")"),
						&ods_table_state::replace_cell_row,
						boost::match_default | boost::format_all);
					odf_formula = res;
				}
				cell->table_table_cell_attlist_.table_formula_ = odf_formula;
				
				cells_.back().empty = false;
			}
		}
	}
	

}

void ods_table_state::add_child_element(office_element_ptr & child_element)
{
	office_table_->add_child_element(child_element);
}

void ods_table_state::convert_position(oox_table_position & oox_pos, double & x, double & y)//c 0 отсчет
{
	double sz_col=0;
	int curr_col = 0,i;
	for (i=0; i< columns_.size(); i++)
	{
		if (oox_pos.col > columns_[i].repeated +  curr_col)
		{
			sz_col += (columns_[i].repeated ) * columns_[i].size;
		}
		else
		{
			sz_col += (oox_pos.col - curr_col ) * columns_[i].size;
			break;
		}
		curr_col += columns_[i].repeated;
	}

	x= sz_col + oox_pos.col_off;

	double sz_row=0;
	int curr_row =0;
	for (i=0; i< rows_.size(); i++)
	{
		if (oox_pos.row > rows_[i].repeated + curr_row)
		{
			sz_row += (rows_[i].repeated ) * rows_[i].size;
		}
		else
		{
			sz_row += (oox_pos.row - curr_row) * rows_[i].size;
			break;
		}
		curr_row += rows_[i].repeated;
	}

	y= sz_row + oox_pos.row_off;
}


void ods_table_state::set_cell_text(odf_text_context* text_context, bool cash_value)
{
	if (text_context == NULL)return;

	for (long i=0; i< text_context->text_elements_list_.size(); i++)
	{
		if (text_context->text_elements_list_[i].level ==0)
		{
			cells_.back().elm->add_child_element(text_context->text_elements_list_[i].elm);
		}
	}
	style* style_ = dynamic_cast<style*>(cells_.back().style_elm.get());
	if (!style_)return;	
	
	odf::style_table_cell_properties	* table_cell_properties = style_->style_content_.get_style_table_cell_properties();

	if (table_cell_properties)
	{
		table_cell_properties->style_table_cell_properties_attlist_.style_text_align_source_ = odf::text_align_source(odf::text_align_source::Fix);
	}	
	
	if (cash_value == false)
	{
		_CP_OPT(office_value_type) cell_type = office_value_type(office_value_type::String);

		table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
		if (cell)
		{
			if (!cell->table_table_cell_attlist_.common_value_and_type_attlist_)
			{
				cell->table_table_cell_attlist_.common_value_and_type_attlist_ = common_value_and_type_attlist();
			}
			//if (!cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_value_type_)
			//{
			cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_value_type_ = cell_type;
			//}
		}
	}
}
void ods_table_state::set_cell_value(std::wstring & value)
{
	if (cells_.size() < 1)return;

	table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
	if (cell == NULL)return;

	if (!cell->table_table_cell_attlist_.common_value_and_type_attlist_)
	{
		cell->table_table_cell_attlist_.common_value_and_type_attlist_ = common_value_and_type_attlist();
		cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_value_type_ = office_value_type(office_value_type::Float);
		//временно... пока нет определ€лки типов
	}
	cells_.back().empty = false;
	
	if (cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_value_type_)
	{
		switch(cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_value_type_->get_type())
		{
		case office_value_type::String:
			cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_string_value_ = value;
			break;
		case office_value_type::Boolean:
			cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_boolean_value_ = value;
			break;
		case office_value_type::Date:
			cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_date_value_ = utils::convert_date(value);
			break;
		case office_value_type::Time:
			cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_time_value_ = utils::convert_time(value);
			break;
		case office_value_type::Currency:
		case office_value_type::Percentage:
		case office_value_type::Float:
		default:
			cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_value_ = value;
		}
	}
	else
	{
		//general !!
	}
	
	//кэшированные значени€ 
	if (value.length() >0)
	{
		bool need_test_cach = false;

		if (cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_value_type_)
		{
			if (cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_value_type_->get_type() == office_value_type::Float) need_test_cach = true;
 			if (cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_value_type_->get_type() == office_value_type::Currency) need_test_cach = true;
			if (cell->table_table_cell_attlist_.common_value_and_type_attlist_->office_value_type_->get_type() == office_value_type::Percentage) need_test_cach = true;
		}
		try
		{
			if (need_test_cach)
			{
				double test = boost::lexical_cast<double>(value);
			}
			context_.start_text_context();
				context_.text_context()->start_paragraph();
					context_.text_context()->add_text_content(value);
				context_.text_context()->end_paragraph();

				set_cell_text(context_.text_context(), true);
			context_.end_text_context();
		}
		catch(...)
		{
		}
	}
}

void ods_table_state::end_cell()
{
	if ( cells_.size() <1)return;

	if (cells_.back().comment_idx >=0)
	{
		office_element_ptr & comm_elm = comments_[cells_.back().comment_idx].elm;
		cells_.back().elm->add_child_element(comm_elm);
	}
	if (cells_.back().empty)
	{
		table_table_cell* cell = dynamic_cast<table_table_cell*>(cells_.back().elm.get());
		if (cell)cell->table_table_cell_attlist_.common_value_and_type_attlist_ = boost::none;
	}
}

void ods_table_state::add_default_cell( int repeated)
{
	int comment_idx = is_cell_comment(current_table_column_+1 , current_table_row_, repeated);
	if (comment_idx  >=0 && repeated >1)
	{
		//делим на 3 - до, с комметом, после;
		int c = current_table_column_;

		add_default_cell(comments_[comment_idx].col - c -1);
		add_default_cell(1);
		add_default_cell(repeated + c +1 - comments_[comment_idx].col);

		return;
	}

//////////////////////////////////////////////////
	office_element_ptr default_cell_elm;
	create_element(L"table", L"table-cell",default_cell_elm, &context_);

	current_row_element()->add_child_element(default_cell_elm);
	
	table_table_cell* cell = dynamic_cast<table_table_cell*>(default_cell_elm.get());
	if (cell == NULL)return;
	
 	ods_cell_state state;
	
	state.empty = true;
	state.elm = default_cell_elm;  state.repeated = repeated; 
	state.row=current_table_row_;  state.col =current_table_column_+1; 
	state.hyperlink_idx = is_cell_hyperlink(state.col, current_table_row_);
	state.comment_idx = comment_idx;
	
	cells_.push_back(state);
	
	current_table_column_+= state.repeated;

	cell->table_table_cell_attlist_.table_number_columns_repeated_ = repeated;

	if (row_default_cell_style_name_.length() > 0)
		cell->table_table_cell_attlist_.table_style_name_ = row_default_cell_style_name_;

	end_cell();
}
///////////////////////////////////////////////////
void ods_table_state::start_conditional_formats()
{
	office_element_ptr		elm;
	create_element(L"calcext", L"conditional-formats",elm,&context_);

	current_level_.back()->add_child_element(elm);
	current_level_.push_back(elm);

}
void ods_table_state::end_conditional_formats()
{
	current_level_.pop_back();
}
void ods_table_state::start_conditional_format(std::wstring ref)
{
	office_element_ptr		elm;
	create_element(L"calcext", L"conditional-format",elm,&context_);

	current_level_.back()->add_child_element(elm);
	current_level_.push_back(elm);

	calcext_conditional_format* cond_format = dynamic_cast<calcext_conditional_format*>(elm.get());

	if (cond_format)
	{
		formulasconvert::oox2odf_converter converter;
		std::wstring out = converter.convert_ref(ref);
		boost::algorithm::replace_all(out,L"[",L"");
		boost::algorithm::replace_all(out,L"]",L"");
		cond_format->calcext_target_range_address_ = out;
		//проверить конвертацию на диапазонах с именами листов в кавычках и с пробелами
	}
}
void ods_table_state::end_conditional_format()
{
	current_level_.pop_back();
}
void ods_table_state::start_conditional_rule(int rule_type)
{
	office_element_ptr		elm;

	switch(rule_type)
	{
	case 3: /*colorScale*/
		create_element(L"calcext", L"color-scale",elm,&context_); break;
	case 7: /*dataBar*/
		create_element(L"calcext", L"data-bar",elm,&context_);	break;
	case 10: /*iconSet*/
		create_element(L"calcext", L"icon-set",elm,&context_);	break;
	case 14: /*timePeriod*/
		create_element(L"calcext", L"date-is",elm,&context_); break;

	case 0: /*aboveAverage*/
	case 1: /*beginsWith*/
	case 2: /*cellIs*/
	case 4: /*containsBlanks*/
	case 5: /*containsErrors*/
	case 6: /*containsText*/
	case 8: /*duplicateValues*/
	case 9: /*expression*/
	case 11: /*notContainsBlanks*/
	case 12: /*notContainsErrors*/
	case 13: /*notContainsText*/
	case 15: /*top10*/
	case 16: /*uniqueValues*/
		default:	
			{
				create_element(L"calcext", L"condition",elm,&context_);
				calcext_condition* condition				= dynamic_cast<calcext_condition*>	 (elm.get());
				calcext_conditional_format* condition_fmt	= dynamic_cast<calcext_conditional_format*>	 (current_level_.back().get());
				if (condition && condition_fmt)
				{
					condition->calcext_condition_attr_.calcext_base_cell_address_=condition_fmt->calcext_target_range_address_;
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
	calcext_condition* condition		 = dynamic_cast<calcext_condition*>	 (current_level_.back().get());

	if (condition)condition->calcext_condition_attr_.calcext_value_= formula;
}
void ods_table_state::set_conditional_style_name(std::wstring style_name)
{
	calcext_condition*	condition	 = dynamic_cast<calcext_condition*>	 (current_level_.back().get());
	calcext_date_is*	date_is		 = dynamic_cast<calcext_date_is*>	 (current_level_.back().get());

	if (condition)condition->calcext_condition_attr_.calcext_apply_style_name_= style_ref(style_name);
	if (date_is) date_is->calcext_date_is_attr_.calcext_style_ = style_ref(style_name);

}
void ods_table_state::set_conditional_value(int type, std::wstring value )
{
	calcext_icon_set* icon_set		 = dynamic_cast<calcext_icon_set*>	 (current_level_.back().get());
	calcext_data_bar* data_bar		 = dynamic_cast<calcext_data_bar*>	 (current_level_.back().get());
	calcext_color_scale* color_scale = dynamic_cast<calcext_color_scale*>(current_level_.back().get());

	if (icon_set || data_bar)
	{
		office_element_ptr		elm;
		create_element(L"calcext", L"formatting-entry",elm, &context_);
		
		current_level_.back()->add_child_element(elm);

		calcext_formatting_entry * entry = dynamic_cast<calcext_formatting_entry*>(elm.get());
		if (entry)
		{
			switch(type)
			{
				case 0: //Formula	
				case 1: entry->calcext_type_ = calcext_type(calcext_type::AutoMaximum); break;
				case 2: entry->calcext_type_ = calcext_type(calcext_type::AutoMinimum); break;
				case 4: entry->calcext_type_ = calcext_type(calcext_type::Percent); break;
				case 5: //Percentile		
				case 3: //Number
				default: entry->calcext_type_ = calcext_type(calcext_type::Number);
			}
		}
	}

	if (color_scale)
	{
		office_element_ptr		elm;
		create_element(L"calcext", L"color-scale-entry",elm, &context_);
		
		current_level_.back()->add_child_element(elm);

		calcext_color_scale_entry * entry = dynamic_cast<calcext_color_scale_entry*>(elm.get());
		if (entry)
		{
			switch(type)
			{				
				case 0: //Formula	
				case 1: entry->calcext_type_ = calcext_type(calcext_type::Maximum); break;
				case 2: entry->calcext_type_ = calcext_type(calcext_type::Minimum); break;
				case 4: entry->calcext_type_ = calcext_type(calcext_type::Percent); break;
				case 5: //Percentile		
				case 3: //Number
				default: entry->calcext_type_ = calcext_type(calcext_type::Number);
			}
		}
		///color???? - прихоод€т выше уровнем !!
	}

}
void ods_table_state::set_conditional_iconset(int type_iconset)
{
	calcext_icon_set* cond_format = dynamic_cast<calcext_icon_set*>(current_level_.back().get());

	if (cond_format)
	{
		cond_format->calcext_icon_set_attr_.calcext_icon_set_type_ = iconset_type((iconset_type::type)type_iconset);
	}
}
void ods_table_state::add_conditional_colorscale(_CP_OPT(color) color)
{
}
void ods_table_state::set_conditional_databar_color(_CP_OPT(color) color)
{
	calcext_data_bar* cond_format = dynamic_cast<calcext_data_bar*>(current_level_.back().get());

	if (cond_format)
	{
		cond_format->calcext_data_bar_attr_.calcext_positive_color_ = color;
	}
}

}
}
