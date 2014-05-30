#pragma once

#include "odf_conversion_context.h"
#include "ods_table_context.h"

namespace cpdoccore { 
namespace odf {

class office_spreadsheet;
class odf_text_context;

struct _font_metrix
{
	_font_metrix(){IsCalc = italic = bold = false; font_size = approx_symbol_size =0;}
	bool IsCalc;

	std::wstring font_name;
	double		 font_size;
	bool		 italic;
	bool		 bold;

	double		 approx_symbol_size;//in pt
};

class ods_conversion_context : public odf_conversion_context
{
public:
	ods_conversion_context(package::odf_document * outputDocument);
	
	virtual void start_document();

	void start_sheet(std::wstring & name);
		void set_sheet_dimension(std::wstring & ref);
	void end_sheet();

	void start_columns();
		void add_column(int start_column, int repeated, int level = 0, bool _default = false);
	void end_columns();

	void	calculate_font_metrix(std::wstring name, double size, bool italic, bool bold);

	void start_rows();
		void start_row(int _start_row, int repeated, int level = 0, bool _default = false);
		void end_row();

		void start_cell(std::wstring & ref, int xfd_style);
		void end_cell();
	void end_rows();

	void start_cell_text();
	void add_text_content(std::wstring & text);
	void end_cell_text();

	void add_merge_cells(std::wstring & ref);
	void add_hyperlink(std::wstring & ref, std::wstring & link, std::wstring & display);

	void start_comment		(int col, int row, std::wstring & author);
	void set_comment_rect	(double l, double t, double w, double h);
	void end_comment		();
///////////////////////////////////////////////////////
	ods_table_state &		current_table() { return table_context_.state();}
/////////////////////////////////////////////////////
	virtual void start_text_context();
	virtual void end_text_context();

	virtual odf_drawing_context		* drawing_context()	{return current_table().drawing_context();}
	virtual odf_text_context		* text_context()	{return current_text_context_;}
	
	void start_drawing()		{drawing_context()->start_drawing();}
	void end_drawing()			{drawing_context()->end_drawing();}

	void start_image(std::wstring & image_file_name);
	void end_image(){drawing_context()->end_image();}

	double convert_symbol_width(double val);
	
	void start_defined_expressions();
		void add_defined_range(std::wstring & name,std::wstring & cell_range, int sheet_id);
		void add_defined_expression(std::wstring & name,std::wstring & value, int sheet_id);
	void end_defined_expressions(){}
private:
	_font_metrix				font_metrix_;
	ods_table_context			table_context_;
	
	odf_text_context*			current_text_context_;
	office_spreadsheet*			root_spreadsheet_;
	

};


}
}