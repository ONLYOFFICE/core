#pragma once

#include <string>
#include "odf_style_state.h"
#include "odf_number_styles_context.h"


namespace cpdoccore {
namespace odf {

//class ods_conversion_context;
//class odt_conversion_context;
class odf_conversion_context;


class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

class odf_style_context;
typedef shared_ptr<odf_style_context>::Type odf_style_context_ptr;
/// \class ods_table_context
class odf_style_context
{
public:
    odf_style_context();
	void set_odf_context(odf_conversion_context * Context);

    void create_style(std::wstring name, const style_family style_family, bool automatic = false, bool root = false, int oox_id = -1);
    void create_default_style(const style_family style_family);
	void create_master_style(std::wstring oox_name, int type = 1);

	void add_style(office_element_ptr elm, bool automatic = false, bool root = false, style_family style_family=style_family::None);
	void add_master_style(office_element_ptr & elm);

	office_element_ptr & add_or_find(std::wstring name, const style_family family, bool automatic = false, bool root = false, int id = -1);

	void process_automatic_styles(office_element_ptr root );
	void process_automatic(office_element_ptr root );
	void process_master(office_element_ptr root );
	void process_office(office_element_ptr root );

	void reset_defaults();	
	void add_default(odf_style_state & state)	{current_default_styles_.push_back(state);}

	std::wstring 			find_odf_style_name		(int oox_id_style, const style_family family, bool root = false);
	office_element_ptr		find_odf_style			(int oox_id_style, const style_family family, bool root = false);
	
	std::wstring			find_odf_style_name_default(const style_family family);
	office_element_ptr		find_odf_style_default	(const style_family family);

	std::wstring 			find_conditional_style_name(int oox_id_style, const style_family family);
	office_element_ptr		find_conditional_style		(int oox_id_style, const style_family family);
	
	bool find_odf_style_state(int oox_id_style, const style_family family, odf_style_state *& state, bool root = false);

    odf_style_state & last_state();
    odf_style_state * last_state(style_family family);

	odf_number_styles_context & numbers_styles() {return number_styles_context_;}
	
	std::wstring find_free_name(const style_family  family);

private:
	odf_number_styles_context	number_styles_context_;
    odf_conversion_context *	odf_context_;
    
	std::vector<odf_style_state>		style_state_list_;
 	std::vector<office_element_ptr>		master_style_list_;
	
	std::vector<odf_style_state> current_default_styles_;//для внутренней работы .. переодически очищаемый частично или полностью
   

	std::wstring get_name_family(const style_family & family);

};


}
}
