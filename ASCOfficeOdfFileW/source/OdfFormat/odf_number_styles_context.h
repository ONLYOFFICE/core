#pragma once

#include <string>
#include <vector>
#include <boost/unordered_map.hpp>

#include "office_elements.h"
#include "office_elements_create.h"

#include "officevaluetype.h"

namespace cpdoccore {
namespace odf {

class ods_conversion_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

struct number_format_state
{
	int oox_num_fmt;//дефолтные (по документации - номера 0-163, за исключением некоторых)

	office_value_type::type ods_type;

	std::vector<std::wstring> format_code;

	std::wstring style_name;

	std::wstring language_code;
	std::wstring currency_str;
};

class odf_number_styles_context
{
public:
    odf_number_styles_context(odf_conversion_context & Context);

	number_format_state		&	add_or_find(int oox_num_fmt, std::wstring formatCode = L"");
	void process_styles(office_element_ptr root );

private:
	std::vector<number_format_state> number_format_array_;
	boost::unordered_map<int, int> named_link_map_;

	void create(int oox_num_fmt, std::wstring formatCode = L""); 	
	void create_default(int oox_num_fmt, std::wstring formatCode = L"");
	void create_style(number_format_state & state);	

	void detect_format(number_format_state & state);

////////////////
	odf_conversion_context & context_;
	std::vector<office_element_ptr>	styles_elments;

//////////////////
	void create_number_style(number_format_state & state, office_element_ptr & root_elm);
	void create_currency_style(number_format_state & state, office_element_ptr & root_elm);
	void create_date_style(number_format_state & state, office_element_ptr & root_elm);
	void create_time_style(number_format_state & state, office_element_ptr & root_elm);
	void create_boolean_style(number_format_state & state, office_element_ptr & root_elm);
	void create_text_style(number_format_state & state, office_element_ptr & root_elm);
	void create_percentage_style(number_format_state & state, office_element_ptr & root_elm);


};
}
}
