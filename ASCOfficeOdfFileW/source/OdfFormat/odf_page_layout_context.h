#pragma once

#include <string>

#include "odf_page_layout_state.h"
#include "length.h"

namespace cpdoccore {
namespace odf {

class odf_style_context;
class odf_conversion_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

class odf_page_layout_context;
typedef shared_ptr<odf_page_layout_context>::Type odf_page_layout_context_ptr;

class odf_page_layout_context
{
public:
    odf_page_layout_context(odf_conversion_context * Context);
	void set_styles_context	(odf_style_context * Context);

	void create_master_page(std::wstring oox_name);
	void create_layout_page();

    odf_layout_state & last_layout();
    odf_master_state & last_master();

	void set_page_margin(_CP_OPT(double) top, _CP_OPT(double) left, _CP_OPT(double) bottom, _CP_OPT(double) right, _CP_OPT(double) header, _CP_OPT(double) footer);
	void set_page_margin(_CP_OPT(length) top, _CP_OPT(length) left, _CP_OPT(length) bottom, _CP_OPT(length) right);
	void set_page_orientation(int type);
	void set_page_size(_CP_OPT(length) width, _CP_OPT(length) height);

private:

	style_page_layout_properties *get_properties();

    odf_conversion_context	* odf_context_;
    odf_style_context		* style_context_;
    
	std::vector<odf_layout_state> layout_state_list_;
 	std::vector<odf_master_state> master_state_list_;
	
};


}
}
