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

	void add_merge_cells(std::wstring & ref);

	ods_table_state & current_table() { return ods_table_context_.state();}
	odf_text_context* current_text_context(){return current_text_context_;}

	void start_text_context();
	void end_text_context();

	void start_text_paragraph();//в ооо иногда параграфы для теста не предусмотрены
	void end_text_paragraph();
	void add_text_content(std::wstring & text);


private:
	ods_table_context ods_table_context_;
	
	odf_text_context*	current_text_context_;

	office_spreadsheet* current_spreadsheet_;

};


}
}