#pragma once

#include <string>
#include <vector>

//#include "ods_drawing_context.h"
//#include "ods_comments_context.h"
//#include "ods_hyperlinks.h"

#include "office_elements.h"
#include "office_elements_create.h"


namespace cpdoccore {
namespace odf
{

class odf_conversion_context;
class odf_style_context;
class paragraph;

class odf_drawing_context
{
public:
	odf_drawing_context(odf_conversion_context *odf_context);
    ~odf_drawing_context();

	void set_styles_context(odf_style_context*  styles_context);//для embedded 
    
	void start_drawing();
	void end_drawing();
	
	void start_frame();
	void end_frame();
	
	void start_shape(int type);
	void end_shape();

	void start_image(std::wstring & path);
	void end_image();
		
	void start_element(office_element_ptr & elm);
    void end_element();

	office_element_ptr & get_current_element();

	bool is_exist_content();
//////////////////////////////////////////////////////////////////////////////////////
	void add_path_element(std::wstring command, std::wstring & elm);
	void add_modifier(std::wstring modifier);
	
	void set_viewBox(double W, double H);

	void finalize(office_element_ptr & root_elm);

///////////////////////////////////////////////////////////////////////////////////////

	void set_rect(double x_pt, double y_pt, double width_pt, double height_pt);
	
	void set_name(std::wstring  name);
	void set_z_order(int id);
	
	void set_flip_H(bool bVal);
	void set_flip_V(bool bVal);

	void set_tile(bool bVal);

	void set_rotate(int iVal);

	void set_no_fill();
	void set_solid_fill(std::wstring hexColor);
//////////////////////////////////////////////////////////////
	void start_area_properies();
	void end_area_properies();
	
	void start_line_properies();
	void end_line_properies();
	
	void start_shadow_properies();
	void end_shadow_properies();

private:

    class Impl;
    _CP_PTR(Impl) impl_;

};

}
}