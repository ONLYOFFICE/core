
#include "PtgName.h"
#include "RevNameTabid.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgName::PtgName()
{
}


PtgName::PtgName(const unsigned int index, const PtgDataType data_type)
:	nameindex(index),
	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5))
{
}


BiffStructurePtr PtgName::clone()
{
	return BiffStructurePtr(new PtgName(*this));
}


void PtgName::storeFields(CFRecord& record)
{
	record << nameindex;
}


void PtgName::loadFields(CFRecord& record)
{
	record >> nameindex;
	
	global_info = record.getGlobalWorkbookInfo();
}


void PtgName::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	RevNameTabidPtr tab_id;
	if(!extra_data.empty() && (tab_id = boost::dynamic_pointer_cast<RevNameTabid>(extra_data.front())))
	{

#pragma message("####################### PtgName struct for revisions is not checked")
		Log::info("PtgName struct for revisions is not checked.");
		ptg_stack.push(tab_id->toString());
		extra_data.pop();
		return;
	}

	std::wstring ptg;
	
	if ((global_info) && (nameindex <= global_info->defineNames.size()))
	{
		ptg = global_info->defineNames[nameindex-1];
	}
	
	if(!ptg.empty())
	{
		ptg_stack.push(ptg);
	}
	else
	{
		ptg_stack.push(L"#UNDEFINED_NAME(" + STR::int2wstr(nameindex) + L")!");
	}
}


} // namespace XLS

