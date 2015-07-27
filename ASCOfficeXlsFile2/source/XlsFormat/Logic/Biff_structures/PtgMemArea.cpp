
#include "PtgMemArea.h"
#include "PtgExtraMem.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgMemArea::clone()
{
	return BiffStructurePtr(new PtgMemArea(*this));
}


//void PtgMemArea::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"cce", cce);
//}


void PtgMemArea::storeFields(CFRecord& record)
{
#pragma message("####################### PtgMemArea record has no BiffStructure::store() implemented")
	Log::error(" Error!!! PtgMemArea record has no BiffStructure::store() implemented.");
	//record << something;
}


void PtgMemArea::loadFields(CFRecord& record)
{
	record.skipNunBytes(4); // unused
	record >> cce;
}


void PtgMemArea::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	PtgExtraMemPtr range;
	if(extra_data.empty() || !(range = boost::dynamic_pointer_cast<PtgExtraMem>(extra_data.front())))
	{
		Log::error("PtgExtraMem must follow the corresponding PtgMemArea.");
		return;
	}
//	ptg_stack.push("#CACHE(" + STR::int2str(getOffsetInRecord() + getSizeOfStruct() + cce, 10) + ")!" + range->toString());
	extra_data.pop();
}


} // namespace XLS

