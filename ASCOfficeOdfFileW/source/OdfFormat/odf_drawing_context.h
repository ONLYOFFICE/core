#pragma once

#include <string>
#include <vector>
//
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

struct odf_drawing_state
{
	office_element_ptr	elm;
	std::wstring		style_name;

	office_element_ptr	style_elm;

	int level;
};

class odf_drawing_context: boost::noncopyable
{
public:
	odf_drawing_context(odf_style_context * styles_context_,odf_conversion_context *odf_context);
    ~odf_drawing_context();
public:
	void set_styles_context(odf_style_context*  styles_context);//для embedded 
    
	void start_drawing();
	void end_drawing();
	
	void start_image(std::wstring & path);
	void end_image();
		
	void start_element(office_element_ptr & elm);
    void end_element();

	std::vector<office_element_ptr> current_level_;//постоянно меняющийся список уровней наследования

	std::vector<odf_drawing_state> drawing_elements_list_;//все элементы
private:

	odf_style_context * styles_context_;
	odf_conversion_context *odf_context_;
};

}
}