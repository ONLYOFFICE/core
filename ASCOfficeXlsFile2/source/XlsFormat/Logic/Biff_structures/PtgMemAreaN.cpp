
#include "PtgMemAreaN.h"
#include "PtgExtraMem.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgMemAreaN::clone()
{
	return BiffStructurePtr(new PtgMemAreaN(*this));
}


void PtgMemAreaN::storeFields(CFRecord& record)
{
	record << cce;
}


void PtgMemAreaN::loadFields(CFRecord& record)
{
	record >> cce;
}


void PtgMemAreaN::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	PtgExtraMemPtr range;
	if(extra_data.empty() || !(range = boost::dynamic_pointer_cast<PtgExtraMem>(extra_data.front())))
	{
		Log::error("PtgMemAreaN must follow the corresponding PtgMemArea (I suppose).");
		return;
	}
	//	ptg_stack.push("#CACHE(" + STR::int2str(getOffsetInRecord() + getSizeOfStruct() + cce, 10) + ")!" + range->toString());
	extra_data.pop();
}




} // namespace XLS

