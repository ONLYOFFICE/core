#pragma once

#include <string>

#include "odf_page_layout_state.h"
#include "length.h"
#include "color.h"


namespace cpdoccore {
namespace odf {

class odf_conversion_context;

class odf_style_context;
typedef shared_ptr<odf_style_context>::Type odf_style_context_ptr;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

class odf_page_layout_context;
typedef shared_ptr<odf_page_layout_context>::Type odf_page_layout_context_ptr;

class odf_page_layout_context
{
public:
    odf_page_layout_context(odf_conversion_context * Context);
    ~odf_page_layout_context();

	void set_styles_context	(odf_style_context * Context);

	void create_master_page(std::wstring oox_name);
	void create_layout_page();

	void set_current_master_page_base();

	void process_master_styles			(office_element_ptr root );
	void process_automatic_for_styles	(office_element_ptr root );
	void process_office_styles			(office_element_ptr root );//картинки, шрифты, ... для мастер пэйджей и шаблонов

    odf_layout_state & last_layout();
    odf_master_state & last_master();

	odf_style_context* get_local_styles_context() //для автоматических стилей самих стилей
		{return local_style_context_.get();}

	void set_page_margin(_CP_OPT(double) top, _CP_OPT(double) left, _CP_OPT(double) bottom, _CP_OPT(double) right, _CP_OPT(double) header, _CP_OPT(double) footer);
	void set_page_margin(_CP_OPT(length) top, _CP_OPT(length) left, _CP_OPT(length) bottom, _CP_OPT(length) right);
	void set_page_gutter(_CP_OPT(length) length_);
	
	void set_page_border(std::wstring top, std::wstring left, std::wstring bottom, std::wstring right);
	void set_page_border_shadow(bool val);
	
	void set_page_border_padding_bottom(int offset_type, double length_pt);
	void set_page_border_padding_top(int offset_type, double length_pt);
	void set_page_border_padding_left(int offset_type, double length_pt);
	void set_page_border_padding_right(int offset_type, double length_pt);

	void set_page_orientation(int type);
	void set_page_size(_CP_OPT(length) width, _CP_OPT(length) height);

	void set_title_page_enable(bool val);
	void set_pages_mirrored(bool val);
	
	void add_footer(int type);
		void set_footer_size(_CP_OPT(length) length_);
	void add_header(int type);
		void set_header_size(_CP_OPT(length) length_);

	void set_background(_CP_OPT(color) & color, int type);

private:

	style_page_layout_properties	*get_properties();
	style_header_footer_properties	*get_header_properties();
	style_header_footer_properties	*get_footer_properties();

    odf_conversion_context	* odf_context_;
    odf_style_context		* style_context_;
    odf_style_context_ptr	  local_style_context_;
  
	std::vector<odf_layout_state> layout_state_list_;
 	std::vector<odf_master_state> master_state_list_;
	
};


}
}
