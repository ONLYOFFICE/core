#include "precompiled_cpodf.h"


#include "ods_conversion_context.h"

#include "office_spreadsheet.h"

namespace cpdoccore { 
namespace odf {

//////////////////////////////////////////////////////////////////////////////////////////////////

void ods_conversion_context::start_document()
{
	create_element(L"office", L"spreadsheet",content_, this);

	//current_spreadsheet_ = (office_spreadsheet*)dynamic_cast<office_spreadsheet*>(content_.back().get());
}

void ods_conversion_context::start_sheet(std::wstring & name)
{
	create_element(L"table", L"table",content_,this/*current_spreadsheet_.getContent()*/);
	
	//current_sheet_ = (table_table*)dynamic_cast<table_table*>(content_.back().get());
	
}

void ods_conversion_context::end_sheet()
{

}



}
}
