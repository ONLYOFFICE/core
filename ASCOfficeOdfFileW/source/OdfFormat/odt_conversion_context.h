#pragma once

#include "odf_conversion_context.h"

#include "odf_comment_context.h"
#include "odf_table_context.h"

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
	odf_table_context				* table_context();
	
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

	void start_run(bool styled = false);
	void end_run();

	void add_section();
	void add_section_columns(int count, double space_pt, bool separator );
	void add_section_column(std::vector<std::pair<double,double>> width_space);

	void start_drop_cap(style_paragraph_properties * paragraph_properties);
		void set_drop_cap_lines(int lines);
		void set_drop_cap_margin(bool val);
	void end_drop_cap();

	bool start_comment(int oox_comment_id);
	void end_comment(int oox_comment_id);

	void start_comment_content();
	void end_comment_content();

	void start_table(bool styled = false);
		void start_table_columns();
			void add_table_column(double width = -1);
		void end_table_columns();
		void start_table_header_rows();
		void end_table_header_rows();
		void start_table_row(bool styled = false);
			void add_default_cell();
			void start_table_cell(int col, bool covered, bool styled = false);
			void end_table_cell();
		void end_table_row();
	void end_table();

private:
	office_text*				root_text_;
	office_element_ptr			root_document_;
	
	odf_text_context*			main_text_context_;

	odf_page_layout_context		page_layout_context_;	
	odf_table_context			table_context_;
	odf_comment_context			comment_context_;	
	
	std::vector<odf_drawing_context_ptr>	drawing_context_;	
	std::vector<odf_text_context_ptr>		text_context_;//for embedded 

	std::vector<office_element_ptr> current_root_elements_; // for section, if needed
	std::vector<odt_section_state>	sections_;

	void add_to_root();

	struct _field_state
	{
		bool enabled;	
		int type;
		std::wstring value;
		bool started;
	}current_field_;

	bool is_hyperlink_;

	struct _drop_cap_state
	{
		void clear(){enabled = false; paragraph_properties = NULL;	characters = 0; inline_style = true; characters_size_pt =0;}

		bool enabled;
		style_paragraph_properties *paragraph_properties;

		int characters;
		bool inline_style;
		double characters_size_pt;
	}drop_cap_state_;



};


}
}