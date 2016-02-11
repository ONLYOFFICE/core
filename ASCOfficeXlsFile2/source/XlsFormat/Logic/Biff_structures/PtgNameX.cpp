
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

	std::wstring _Name;
	if(nameindex > 0 && nameindex <= global_info->AddinUdfs.size() && !(_Name = global_info->AddinUdfs[nameindex - 1]).empty())
	{
		ptg_stack.push(_Name);
	}
	else if(ixti > 0 && ixti <= global_info->xti_parsed.size())
	{
		std::wstring sheet = global_info->xti_parsed[ixti-1];

		if (!sheet.empty()) sheet += L"!";
		
		if (nameindex > 0 && nameindex <= global_info->arDefineNames.size())
		{
			_Name = global_info->arDefineNames[nameindex - 1];
		}

		if (sheet.empty() && _Name.empty() && nameindex <= global_info->arExternalNames.size())
		{
			_Name = global_info->arExternalNames[nameindex - 1];
		}

		ptg_stack.push(sheet + _Name);
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

