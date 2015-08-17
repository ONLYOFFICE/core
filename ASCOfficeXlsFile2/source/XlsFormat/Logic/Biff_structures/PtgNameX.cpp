
#include "PtgNameX.h"
#include "RevName.h"
#include <Binary/CFRecord.h>

namespace XLS
{


PtgNameX::PtgNameX()
{
}


PtgNameX::PtgNameX(const std::wstring& word, const PtgDataType data_type)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5))
{
	fromString(word);
}


BiffStructurePtr PtgNameX::clone()
{
	return BiffStructurePtr(new PtgNameX(*this));
}


void PtgNameX::storeFields(CFRecord& record)
{
	record << ixti << nameindex;
}


void PtgNameX::loadFields(CFRecord& record)
{
	record >> ixti >> nameindex;
	global_info = record.getGlobalWorkbookInfo();
}


void PtgNameX::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	RevNamePtr tab_id;
	if(!extra_data.empty() && (tab_id = boost::dynamic_pointer_cast<RevName>(extra_data.front())))
	{
#pragma message("####################### PtgNameX struct for revisions is not implemented")
		Log::error("PtgNameX struct for revisions is not implemented.");
		ptg_stack.push(L"#REF!");
		extra_data.pop();
		return;
	}

	std::wstring udfName;
	if(nameindex > 0 && nameindex <= global_info->AddinUdfs.size() && !(udfName = global_info->AddinUdfs[nameindex - 1]).empty())
	{
		ptg_stack.push(udfName);
	}
	else
	{
#pragma message("####################### PtgNameX struct is not implemented")
 		Log::warning("PtgNameX structure is not implemented.");
		//ptg_stack.push(L"#UNDEFINED_EXTERN_NAME(" + STR::int2wstr(nameindex) + L")!");
		ptg_stack.push(L""); // This would let us to continue without an error
	}


	// Example of result: "[1]!range"
	// in the formula window it looks like: "'D:\Projects\AVSWorksheetConverter\bin\InFiles\Blank2003_range.xls'!range"
}


void PtgNameX::fromString(const std::wstring& word)
{
 	std::wstring  query = L"root/WorkbookStreamObject/GlobalsSubstream/SUPBOOK";
}


} // namespace XLS

