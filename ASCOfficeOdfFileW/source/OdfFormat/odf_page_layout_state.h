#pragma once

#include <string>
#include <vector>

#include "office_elements.h"
#include "office_elements_create.h"


namespace cpdoccore {
namespace odf {

class odf_conversion_context;
class odf_style_context;
class odf_page_layout_context;
class style_page_layout_properties;

struct 	odf_element_state
{
	office_element_ptr	elm;
	std::wstring		style_name;

	office_element_ptr	style_elm;
	
	int level;
};

class odf_master_state
{
public:
	odf_master_state(office_element_ptr & master);

	void add_child(office_element_ptr & child_elm, office_element_ptr  style_elm, std::wstring style_name );

	void			set_name(std::wstring & name);
	std::wstring	get_name();

	void add_footer(office_element_ptr & elm);
	void add_header(office_element_ptr & elm);
	
	void			set_layout_name(std::wstring & name);
	
private:
	std::wstring	style_oox_name_;

	std::vector<office_element_ptr>	current_level_;
	std::vector<odf_element_state>	elements_;

	friend class odf_page_layout_context;
};

class odf_layout_state
{
public:
	odf_layout_state(office_element_ptr & elm);

	void add_child(office_element_ptr & child_elm, office_element_ptr  style_elm, std::wstring style_name );

	void			set_name(std::wstring & name);
	std::wstring	get_name();	

	style_page_layout_properties * get_properties();//пока не красиво ... лучше хранить прямую ссыль нежели искать
private:
	std::wstring	style_oox_name_;

	std::vector<office_element_ptr>	current_level_;
	std::vector<odf_element_state>	elements_;

	friend class odf_page_layout_context;
};

}
}

