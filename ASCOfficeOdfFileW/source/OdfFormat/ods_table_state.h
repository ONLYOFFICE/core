#pragma once

#include <string>
#include <vector>

#include <boost/regex.hpp>

#include "odf_drawing_context.h"

#include "office_elements.h"
#include "office_elements_create.h"

#include "officevaluetype.h"


namespace cpdoccore {

struct oox_table_position
{
    int col;    
    double col_off;
    int row;
    double row_off;
};

namespace odf {

class ods_conversion_context;
class odf_text_context;

class table_table;
class style;
class color;

namespace utils {
	static std::wstring getColAddress(size_t col)
	{
		static const size_t r = (L'Z' - L'A' + 1);
		std::wstring res;
		size_t r0 = col / r;

		if (r0 > 0)
		{
			const std::wstring rest = getColAddress(col - r*r0);
			const std::wstring res = getColAddress(r0-1) + rest;
			return res;
		}
		else
			return std::wstring(1, (wchar_t)(L'A' + col));
	}
	static size_t getColAddressInv(const std::wstring & a_)
	{
		std::wstring a = a_;
		::boost::algorithm::to_upper(a);
		static const size_t r = (L'Z' - L'A' + 1);
		size_t mul = 1;
		bool f = true;
		size_t res = 0;
		BOOST_REVERSE_FOREACH(const wchar_t c, a)
		{
			size_t v = c - L'A';
			if (f)
				f = false;
			else
				v += 1;
			res += v * mul;
			mul *= r;
		}
		return res;
	}
	static size_t getRowAdderssInv(const std::wstring & a_)
	{
		int sz = a_.length();
		if (a_.length()>0)
		{
		   return boost::lexical_cast<size_t>(a_)-1;
		}
		else
			return 0;
	}
	static void splitCellAddress(const std::wstring & a_, std::wstring & col, std::wstring & row)
	{   
		std::wstring a = a_;

		std::reverse(a.begin(), a.end());
		::boost::algorithm::replace_all(a, L"$", L"");
		//::boost::algorithm::replace_all(a, L"'", L"");
		::boost::algorithm::to_upper(a);
		

		BOOST_FOREACH(wchar_t c, a)
		{
			if (c >= L'0' && c <= L'9')
				row +=c;
			else
				col += c;
		}
		std::reverse(col.begin(), col.end());
		std::reverse(row.begin(), row.end());
	}
	static void parsing_ref (const std::wstring & ref, int & col,int & row)
	{
		std::wstring strCol, strRow;
		splitCellAddress(ref,strCol,strRow);

		col = getColAddressInv(strCol)+1;
		row = getRowAdderssInv(strRow)+1;

	}
};
struct ods_element_state
{
	office_element_ptr elm;
	
	int repeated;
	std::wstring style_name;
	office_element_ptr style_elm;

	double size;

	int level;
};

struct ods_cell_state : ods_element_state
{
	int col;
	int row;

	int hyperlink_idx;
	int comment_idx;
	
	bool empty;
};

struct ods_hyperlink_state
{
	std::wstring ref;
	int col;
	int row;
	std::wstring link;
};
struct ods_comment_state
{
	int col;
	int row;
	std::wstring author;

	office_element_ptr elm;
	//style graphic
};
struct ods_shared_formula_state
{
	int index;
	std::wstring formula;
	std::wstring ref;

	int	base_column;
	int	base_row;

	int moving_type; //1 - col, 2 - row
};

class ods_table_state
{
public:
	ods_table_state(ods_conversion_context & Context, office_element_ptr & elm);
		void set_table_name(std::wstring name);
		void set_table_style(office_element_ptr & _style);
		void set_table_hidden(bool Val);
		void set_table_tab_color(_CP_OPT(color) & _color);
		void set_table_dimension(int col, int row);
		void set_print_range(std::wstring range);

	void add_column(office_element_ptr & elm, int repeated ,office_element_ptr & style);
		void set_column_width(double width);
		void set_column_optimal_width(bool val);
		void set_column_default_cell_style(std::wstring & style_name);

	void start_group(office_element_ptr & elm);
	void end_group();
		
	int current_level() {return current_level_.size()-1;}

	void add_row(office_element_ptr & elm, int repeated ,office_element_ptr & style);//const std::wstring & StyleName, const std::wstring & defaultCellStyleName);
		void set_row_hidden(bool Val);
		void set_row_optimal_height(bool val);
		void set_row_height(double height);
		void set_row_default_cell_style(std::wstring & style_name);

	void start_cell(office_element_ptr & elm ,office_element_ptr & style);
	void end_cell();
	void add_default_cell(int repeated);

	void set_cell_format_value(office_value_type::type value_type);
	void set_cell_type(int type);
	void set_cell_value(std::wstring & value);	
	void set_cell_text(odf_text_context *text_context, bool cash_value = false);
	void set_cell_formula(std::wstring &formula);
	void add_or_find_cell_shared_formula(std::wstring & formula, std::wstring ref, int ind);
	
	static std::wstring replace_cell_row(boost::wsmatch const & what);
	static std::wstring replace_cell_column(boost::wsmatch const & what);
	
	void add_child_element(office_element_ptr & child_element);

	void start_conditional_formats();
		void start_conditional_format(std::wstring ref);
			void start_conditional_rule(int rule_type);
				void set_conditional_formula(std::wstring formula);
				void set_conditional_value(int type, std::wstring value );
				void set_conditional_iconset(int type_iconset);
				void add_conditional_colorscale(_CP_OPT(color) color);
				void set_conditional_databar_color(_CP_OPT(color) color);
				void set_conditional_style_name(std::wstring style_name);
			void end_conditional_rule();
		void end_conditional_format();
	void end_conditional_formats();

///////////////////////////////
	void add_hyperlink(std::wstring & ref,int col, int row, std::wstring & link);
	void add_definded_expression(office_element_ptr & elm);

	void start_comment(int col, int row, std::wstring & author);
		void set_comment_rect(double l, double t, double w, double h);
	void end_comment(odf_text_context *text_context);
	
	void set_merge_cells(int start_col, int start_row, int end_col, int end_row);

	office_element_ptr & current_row_element();
	office_element_ptr & current_cell_element();

	bool	is_cell_hyperlink();
	int		is_cell_hyperlink(int col, int row);
	bool	is_cell_comment();
	int		is_cell_comment(int col, int row, int repeate_col = 1);

	ods_hyperlink_state & current_hyperlink();

	int current_column() const;
	int current_row() const;

	int dimension_columns;
	int dimension_row;

	double defaut_column_width_;
	double defaut_row_height_;

	void convert_position(oox_table_position & oox_pos, double & x, double & y);

	odf_drawing_context   *	drawing_context(){return  &drawing_context_;}

private:

    ods_conversion_context & context_;   
	
	std::wstring		office_table_name_;
	office_element_ptr	office_table_;
	style*				office_table_style_;//??? может хранить как office_element_ptr ???
	office_element_ptr	table_defined_expressions_;
	

	std::wstring row_default_cell_style_name_;

	static int current_table_column_;
	static int current_table_row_;

	static int tmp_column_;
	static int tmp_row_;

	std::vector<ods_element_state> columns_;
	std::vector<ods_element_state> rows_;
	
	std::vector<office_element_ptr> current_level_;//постоянно меняющийся список уровней ("0-й элемент - сама таблица)
	
	std::vector<ods_cell_state> cells_;
	
	std::vector<ods_hyperlink_state>	hyperlinks_;
	std::vector<ods_comment_state>		comments_;

	std::vector<ods_shared_formula_state> shared_formulas_;

	odf_drawing_context		drawing_context_;	

	friend class ods_table_context;

};


}
}

