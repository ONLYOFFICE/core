#include "precompiled_cpodf.h"


#include "ods_conversion_context.h"

#include "office_spreadsheet.h"

namespace cpdoccore { 
namespace odf {


ods_conversion_context::ods_conversion_context(package::odf_document * outputDocument) 
		: odf_conversion_context(outputDocument), ods_table_context_(*this)
{
}


void ods_conversion_context::start_document()
{
	create_element(L"office", L"spreadsheet", content_, this,true);

	current_spreadsheet_ = dynamic_cast<office_spreadsheet*>(content_.back().get());
}


void ods_conversion_context::start_sheet(std::wstring & name)
{
	create_element(L"table", L"table",current_spreadsheet_->getContent(),this);
	
	ods_table_context_.start_table(current_spreadsheet_->getContent().back(),name);
	
}

void ods_conversion_context::end_sheet()
{
	ods_table_context_.end_table();
}

void ods_conversion_context::add_column(int start_column, int repeated, const std::wstring & style_name)
{
	if (start_column > ods_table_context_.state().columns_count()+1)
	{
		//default_columns 
		int repeated_default = start_column - ods_table_context_.state().columns_count()-1;
		office_element_ptr element_column_default;
		create_element(L"table", L"table-column",element_column_default,this);
	
		ods_table_context_.add_column(element_column_default,repeated_default,L"");
	}

	office_element_ptr element_column;

	create_element(L"table", L"table-column",element_column,this);
	ods_table_context_.add_column(element_column,repeated,style_name);
}


}
}
