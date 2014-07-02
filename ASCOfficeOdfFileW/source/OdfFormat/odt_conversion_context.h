#pragma once

#include "odf_conversion_context.h"
#include "odf_comment_context.h"

namespace cpdoccore { 
namespace odf {

class office_text;

class odf_text_context;

typedef shared_ptr<odf_text_context>::Type odf_text_context_ptr;
typedef shared_ptr<odf_drawing_context>::Type odf_drawing_context_ptr;

struct odt_section_state
{
	office_element_ptr	elm;
	office_element_ptr	style_elm;
	std::wstring		style_name;

	bool				empty;
};

class odt_conversion_context : public odf_conversion_context
{
public:
	odt_conversion_context(package::odf_document * outputDocument);
	~odt_conversion_context();
	
	virtual void start_document();
	virtual void end_document();

	virtual void start_text_context();
	virtual void end_text_context();

	virtual odf_drawing_context		* drawing_context();
	virtual odf_text_context		* text_context();
		
	odf_comment_context				* comment_context();
	
	void start_drawings();
	void end_drawings();

	void start_image(std::wstring & image_file_name);
	void add_text_content	(std::wstring & text);

	void start_paragraph(bool styled = false);
	void end_paragraph();

	void start_hyperlink(std::wstring ref);
	void end_hyperlink();

	void start_field();
	void end_field();
	void set_field_instr(std::wstring instr);

	void start_run();
	void end_run();

	void add_section();
	void add_section_columns(int count, double space_pt, bool separator );
	void add_section_column(std::vector<std::pair<double,double>> width_space);

	bool start_comment(int oox_comment_id);
	void end_comment(int oox_comment_id);

	void start_comment_content();
	void end_comment_content();

private:
	office_text*				root_text_;
	office_element_ptr			root_document_;
	
	odf_page_layout_context		page_layout_context_;	
	odf_comment_context			comment_context_;	
	odf_text_context*			main_text_context_;
	
	std::vector<odf_drawing_context_ptr>	drawing_context_;	
	std::vector<odf_text_context_ptr>		text_context_;//for embedded 

	std::vector<office_element_ptr> current_paragraphs_; // for section, if needed
	std::vector<odt_section_state>	sections_;

	struct _field
	{
		bool enabled;	
		int type;
		std::wstring value;
		bool started;
	}current_field_;

	bool is_hyperlink_;



};


}
}