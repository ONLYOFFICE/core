#pragma once

#include <string>
#include <vector>

#include "office_elements.h"
#include "office_elements_create.h"


namespace cpdoccore {
namespace odf {

class style_table_cell_properties;

struct table_format_state
{
	std::wstring style_name;

};

class odf_table_styles_context
{
public:
	odf_table_styles_context(){}

	style_table_cell_properties * get_table_cell_properties(int col, int row){return NULL;}

	bool set_current_style(std::wstring name) {return false;}


private:
	std::vector<table_format_state> table_format_array_;

//////////////////


};
}
}
