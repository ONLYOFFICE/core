#pragma once

#include <string>
#include <vector>
#include <boost/unordered_map.hpp>

#include "office_elements.h"
#include "office_elements_create.h"


namespace cpdoccore {
namespace odf {

class ods_conversion_context;
class style_list_level_properties;
class style_list_level_label_alignment;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

struct list_format_state
{
	int								oox_based_number;
	std::vector<office_element_ptr> elements;

	std::wstring					odf_list_style_name;

};

class odf_lists_styles_context
{
public:
    odf_lists_styles_context();
	void set_odf_context(odf_conversion_context * Context);

	void start_style(int abstract_number);
		void start_style_level(int level, int type);
			style_list_level_properties			* get_list_level_properties();
			style_list_level_label_alignment	* get_list_level_alignment_properties();
		void end_style_level();
	void end_style();

	void add_style(int oox_style_num, int oox_based_num);
	
	void process_styles(office_element_ptr root );

	std::wstring get_style_name(int oox_style_num);
	std::wstring get_style_name1(int oox_style_num);
private:
	std::vector<list_format_state> lists_format_array_;
	
	boost::unordered_map<int, std::wstring> link_format_map_;

	//void create(int oox_num_fmt, std::wstring formatCode = L""); 	
	//void create_default(int oox_num_fmt, std::wstring formatCode = L"");
	//void create_style(number_format_state & state);	

	//void detect_format(number_format_state & state);

////////////////
	odf_conversion_context			*odf_context_;
	//std::vector<office_element_ptr>	styles_elments;


};
}
}
