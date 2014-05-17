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
class style_paragraph_properties;
class style_text_properties;

struct odf_text_state
{
	office_element_ptr	elm;
	std::wstring		style_name;

	office_element_ptr	style_elm;

	int level;
};

class odf_text_context: boost::noncopyable
{
public:
	odf_text_context(odf_conversion_context *odf_context);
    ~odf_text_context();
public:
	void set_styles_context(odf_style_context*  styles_context);//дл€ embedded 
    
	void set_single_object(bool val, style_paragraph_properties *para_props, style_text_properties *text_props);
	void add_text_content(const std::wstring & text);
    
	void start_paragraph(bool styled = false);
	void start_paragraph(office_element_ptr & elm, bool styled = false);
    void end_paragraph();

	void start_element(office_element_ptr & elm);
    void end_element();
 	
	void start_span(bool styled = false); 
    void end_span();
		
	std::vector<office_element_ptr> current_level_;//посто€нно мен€ющийс€ список уровней наследовани€

	std::vector<odf_text_state> text_elements_list_;//параграфы, списки , ... 

	void set_parent_paragraph_style	(std::wstring & style_name);
	void set_parent_span_style		(std::wstring & style_name);

	style_text_properties		*get_text_properties(){return text_properties_;}
	style_paragraph_properties	*get_paragraph_properties(){return paragraph_properties_;}
private:
	bool single_paragraph_;
	style_paragraph_properties	*paragraph_properties_;//хранилка-опознавалка что свойства приписаны другому, не текстовому, объекту
	style_text_properties		*text_properties_;//хранилка-опознавалка что свойства приписаны другому, не текстовому, объекту
	
	paragraph * last_paragraph_;

	odf_style_context * styles_context_;
	odf_conversion_context *odf_context_;

	std::wstring parent_span_style_;
	std::wstring parent_paragraph_style_;
};

}
}