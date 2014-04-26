#pragma once

#include "odf_conversion_context.h"
#include "ods_table_context.h"

namespace cpdoccore { 
namespace odf {

class office_spreadsheet;
class odf_text_context;

class ods_conversion_context : public odf_conversion_context
{
public:
	ods_conversion_context(package::odf_document * outputDocument);// : odf_conversion_context(outputDocument){}
	
	virtual void start_document();

	void start_sheet(std::wstring & name);
		void set_sheet_dimension(std::wstring & ref);
	void end_sheet();

	void start_columns();
		void add_column(int start_column, int repeated, int level = 0, bool _default = false);
	void end_columns();

	void start_rows();
		void start_row(int _start_row, int repeated, int level = 0, bool _default = false);
		void end_row();

		void start_cell(std::wstring & ref, int xfd_style);
		void end_cell(){}
	void end_rows();

	void start_cell_text();
	void add_text_content(std::wstring & text);
	void end_cell_text();

	void add_merge_cells(std::wstring & ref);
	void add_hyperlink(std::wstring & ref, std::wstring & link, std::wstring & display);
///////////////////////////////////////////////////////
	ods_table_state &		current_table() { return table_context_.state();}
/////////////////////////////////////////////////////
	void start_text_context();
	void end_text_context();

	virtual odf_drawing_context&	drawing_context(){return current_table().drawing_context();}
	virtual odf_text_context*		text_context(){return current_text_context_;}
	
	void start_drawing(){drawing_context().start_drawing();}
	void end_drawing(){drawing_context().end_drawing();}

	void start_image(std::wstring & image_file_name);
	void end_image(){drawing_context().end_image();}

private:
	ods_table_context			table_context_;
	
	odf_text_context*			current_text_context_;
	office_spreadsheet*			current_spreadsheet_;
	

};


}
}