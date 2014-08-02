#pragma once

#include <string>

#include "odf_style_state.h"
#include "odf_number_styles_context.h"
#include "odf_table_styles_context.h"


namespace cpdoccore {
namespace odf {

class odf_conversion_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

class odf_style_context;
typedef shared_ptr<odf_style_context>::Type odf_style_context_ptr;

class style;
class paragraph_format_properties;

class odf_style_context
{
public:
    odf_style_context();
	void set_odf_context(odf_conversion_context * Context);

	void create_style(std::wstring name, style_family::type family, bool automatic = false, bool root = false, int oox_id = -1);
	void create_default_style(style_family::type family);

	void add_style(office_element_ptr elm, bool automatic = false, bool root = false, style_family::type family=style_family::None);

	office_element_ptr & add_or_find(std::wstring name, style_family::type family, bool automatic = false, bool root = false, int id = -1);

	void process_automatic_for_styles	(office_element_ptr root );
	void process_automatic_styles		(office_element_ptr root );
	void process_office_styles			(office_element_ptr root );

	void reset_defaults();	
	void add_default(odf_style_state_ptr & state)	{current_default_styles_.push_back(state);}

	std::wstring 			find_odf_style_name			(int oox_id_style, style_family::type family, bool root = false);
	office_element_ptr		find_odf_style				(int oox_id_style, style_family::type family, bool root = false);
	
	std::wstring			find_odf_style_name_default	(style_family::type family);
	office_element_ptr		find_odf_style_default		(style_family::type family);

	std::wstring 			find_conditional_style_name	(int oox_id_style, style_family::type family);
	office_element_ptr		find_conditional_style		(int oox_id_style, style_family::type family);
	
	bool find_odf_style_state(int oox_id_style, style_family::type family, odf_style_state_ptr & state, bool root = false);
	
	bool find_odf_style(std::wstring style_name, style_family::type family, style *& style_);

	odf_style_state_ptr	last_state(style_family::type family = style_family::None);

	odf_number_styles_context & numbers_styles() {return number_styles_context_;}
	odf_table_styles_context & table_styles() {return table_styles_context_;}
	
	std::wstring find_free_name(style_family::type  family);

//////////////////////////////////////////////////////////////////////
	void calc_paragraph_properties(std::wstring style_name, style_family::type family, paragraph_format_properties * result);

private:
	odf_table_styles_context	table_styles_context_;
	odf_number_styles_context	number_styles_context_;
    odf_conversion_context *	odf_context_;
    
	std::vector<odf_style_state_ptr>		style_state_list_;	
	std::vector<odf_style_state_ptr>		current_default_styles_;//для внутренней работы .. переодически очищаемый частично или полностью
   

	std::wstring get_name_family(style_family::type family);

};


}
}
