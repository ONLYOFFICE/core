
#include "DXFN.h"
#include <Binary/CFRecord.h>

namespace XLS
{

BiffStructurePtr DXFProt::clone()
{
	return BiffStructurePtr(new DXFProt(*this));
}


void DXFProt::store(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fLocked);
	SETBIT(flags, 1, fHidden);
	record << flags;
}


void DXFProt::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fLocked = GETBIT(flags, 0);
	fHidden = GETBIT(flags, 1);
}

int DXFProt::serialize(std::wostream & stream)
{
	if (parent->lockedNinch && parent->hiddenNinch) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"protect")
		{	
			if (!parent->lockedNinch)
 				CP_XML_ATTR(L"locked", fLocked);
			if (!parent->hiddenNinch )
				CP_XML_ATTR(L"hidden", fHidden);
		}
	}
	return 0;
}


} // namespace XLS

