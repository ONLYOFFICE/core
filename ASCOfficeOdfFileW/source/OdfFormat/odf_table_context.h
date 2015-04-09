#pragma once

#include <string>
#include <vector>

#include <cpdoccore/CPOptional.h>

#include "office_elements_create.h"

namespace cpdoccore {
namespace odf
{

class odf_conversion_context;
class odf_style_context;

class odf_table_context
{
public:
	odf_table_context(odf_conversion_context *odf_context);
    ~odf_table_context();

	void set_styles_context(odf_style_context*  styles_context);
	
	void		set_table_styled(bool val);
	bool		is_styled();

	bool		empty();

	int current_column ();
	int current_row ();
	int count_columns ();
	int count_rows ();
	void count_rows (int count);

	void start_table(office_element_ptr &elm, bool styled = false);
		void set_default_column_width(double width);
		void change_current_column_width(double width);
	void end_table();
	
	void		 set_default_cell_properties(std::wstring style_name);
	std::wstring get_default_cell_properties();
	
	double		 get_table_width();

	void set_table_inside_v(_CP_OPT(std::wstring) border);
	void set_table_inside_h(_CP_OPT(std::wstring) border);
	
	_CP_OPT(std::wstring) get_table_inside_v();
	_CP_OPT(std::wstring) get_table_inside_h();

	void start_row(office_element_ptr &elm, bool styled = false);
	void end_row();

	void start_cell(office_element_ptr &elm, bool styled = false);
		void set_cell_column_span(int spanned);
		void set_cell_row_span();
		void set_cell_row_span_restart();	
	void end_cell();

	void add_column(office_element_ptr &elm, bool styled = false);
		void set_column_width(double width);
		void set_column_optimal(bool val);



private:

    class Impl;
    _CP_PTR(Impl) impl_;

};

}
}