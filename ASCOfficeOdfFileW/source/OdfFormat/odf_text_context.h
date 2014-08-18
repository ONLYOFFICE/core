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
	void				clear_params();
	void				set_styles_context(odf_style_context*  styles_context);//дл€ embedded 
 	odf_style_context*	get_styles_context();//дл€ embedded 
   
	void add_text_style(office_element_ptr & style_elm, std::wstring style_name);

	void set_single_object(bool val, style_paragraph_properties *para_props, style_text_properties *text_props);
	
	void add_text_content(std::wstring & text);
	void add_text_space(int count);

	void set_symbol_font(std::wstring & font);
 	void set_symbol_text(int sym);
   
	void start_paragraph(bool styled = false);
	void start_paragraph(office_element_ptr & elm, bool styled = false);
    void end_paragraph();


	void start_element(office_element_ptr & elm, office_element_ptr style_elm = office_element_ptr(),std::wstring style_name = L"");
    void end_element();
 	
	void start_field(int type);
	void end_field();

	void start_span	(bool styled = false); 
    void end_span();

	void start_list_item();
	void end_list_item();

	void start_list(std::wstring style_name);
	void end_list();

	bool is_need_break(){return bool(need_break_);}
	void set_type_break(int type, int clear);
	void save_property_break();

	void add_tab();

	int current_outline_;
	bool in_field_;

	std::vector<odf_element_state>	current_level_;//посто€нно мен€ющийс€ список уровней наследовани€
	std::vector<odf_element_state>	text_elements_list_;//параграфы, списки , ... 

	void set_parent_paragraph_style	(std::wstring & style_name);
	void set_parent_span_style		(std::wstring & style_name);

	style_text_properties		*get_text_properties(){return text_properties_;}
	style_paragraph_properties	*get_paragraph_properties(){return paragraph_properties_;}

	void set_outline_level(int level);

	bool get_KeepNextParagraph()			{return keep_next_paragraph_;}
	void set_KeepNextParagraph(bool val)	{keep_next_paragraph_ = val;}

	bool get_list_item_state()				{return list_state_.levels.size() > 0 ? list_state_.levels.back() : false;}

	struct _list_state
	{
		bool				started_list;
		std::vector<bool>	levels;
		std::wstring		style_name;
	}list_state_;
private:
	bool	keep_next_paragraph_;

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