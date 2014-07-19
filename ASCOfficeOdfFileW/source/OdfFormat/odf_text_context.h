#pragma once

#include <string>
#include <vector>

#include "odf_page_layout_state.h"
//#include "ods_drawing_context.h"
//#include "ods_comments_context.h"
//#include "ods_hyperlinks.h"

#include "office_elements.h"
#include "office_elements_create.h"

#include "fobreak.h"

namespace cpdoccore {
namespace odf
{

class odf_conversion_context;
class odf_style_context;
class paragraph;
class style_paragraph_properties;
class style_text_properties;

class odf_text_context: boost::noncopyable
{
public:
	odf_text_context(odf_conversion_context *odf_context);
    ~odf_text_context();
public:
	void set_styles_context(odf_style_context*  styles_context);//дл€ embedded 
    
	void add_text_style(office_element_ptr & style_elm, std::wstring style_name);

	void set_single_object(bool val, style_paragraph_properties *para_props, style_text_properties *text_props);
	void add_text_content(const std::wstring & text);
	void set_symbol_font(std::wstring & font);
 	void set_symbol_text(int sym);
   
	void start_paragraph(bool styled = false);
	void start_paragraph(office_element_ptr & elm, bool styled = false);
    void end_paragraph();

	void start_element(office_element_ptr & elm, office_element_ptr style_elm = office_element_ptr(),std::wstring style_name = L"");
    void end_element();
 	
	void start_span(bool styled = false); 
    void end_span();
		
	void add_break(int type, int clear);
	void add_page_break();
	void add_tab();

	int current_outline_;

	std::vector<odf_element_state>	current_level_;//посто€нно мен€ющийс€ список уровней наследовани€
	std::vector<odf_element_state>	text_elements_list_;//параграфы, списки , ... 

	void set_parent_paragraph_style	(std::wstring & style_name);
	void set_parent_span_style		(std::wstring & style_name);

	style_text_properties		*get_text_properties(){return text_properties_;}
	style_paragraph_properties	*get_paragraph_properties(){return paragraph_properties_;}

	void set_outline_level(int level);
private:
	_CP_OPT(fo_break) need_break_; 

	bool single_paragraph_;
	style_paragraph_properties	*paragraph_properties_;//хранилка-опознавалка что свойства приписаны другому, не текстовому, объекту
	style_text_properties		*text_properties_;//хранилка-опознавалка что свойства приписаны другому, не текстовому, объекту
	

	odf_style_context * styles_context_;
	odf_conversion_context *odf_context_;

	std::wstring parent_span_style_;
	std::wstring parent_paragraph_style_;
};

}
}