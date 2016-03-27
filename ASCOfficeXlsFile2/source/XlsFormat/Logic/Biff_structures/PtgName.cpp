
#include "PtgName.h"
#include "RevNameTabid.h"
#include <Binary/CFRecord.h>

namespace XLS
{


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
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		record.skipNunBytes(12);
	}
	else
	{
		record.skipNunBytes(2);
	}
	
	global_info = record.getGlobalWorkbookInfo();
}


void PtgName::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
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
	
	if ((global_info) && (nameindex <= global_info->arDefineNames.size()))
	{
		ptg = global_info->arDefineNames[nameindex-1];
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

