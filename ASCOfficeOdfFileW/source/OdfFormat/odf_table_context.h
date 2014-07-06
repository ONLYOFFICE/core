#pragma once

#include <string>
#include <vector>

#include <cpdoccore/CPOptional.h>

#include "office_elements.h"
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

	void start_table(office_element_ptr &elm, bool styled = false);
	void end_table();

	void start_row(office_element_ptr &elm, bool styled = false);
	void end_row();

	void start_cell(office_element_ptr &elm, bool styled = false);
		void set_cell_spanned(int spanned);
	void end_cell();

	void add_column(office_element_ptr &elm, bool styled = false);
		void set_column_width(double width);

private:

    class Impl;
    _CP_PTR(Impl) impl_;

};

}
}