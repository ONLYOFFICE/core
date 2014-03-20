#pragma once

#include "odf_conversion_context.h"
#include "ods_table_context.h"

namespace cpdoccore { 
namespace odf {

class office_spreadsheet;

class ods_conversion_context : public odf_conversion_context
{
public:
	ods_conversion_context(package::odf_document * outputDocument);// : odf_conversion_context(outputDocument){}
	
	virtual void start_document();

	void start_sheet(std::wstring & name);
	void end_sheet();

	void add_column(int start_column, int repeated, const std::wstring & StyleName);

	ods_table_context ods_table_context_;

	office_spreadsheet* current_spreadsheet_;

	ods_table_state & current_table() { return ods_table_context_.state();}


};


}
}