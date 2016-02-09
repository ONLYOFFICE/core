
#include "DXFN12.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr DXFN12::clone()
{
	return BiffStructurePtr(new DXFN12(*this));
}

void DXFN12::store(CFRecord& record)
{
	if(dxfn)
	{
		record.registerDelayedDataReceiver(NULL, sizeof(unsigned int)/*cbDxf*/, rt_STRUCT_DXFN12);
		size_t offset_dxfn = record.getDataSize();
		record << *dxfn;
		if(xfext)
		{
			record << *xfext;
		}
		record.registerDelayedDataSource(record.getDataSize() - offset_dxfn, rt_STRUCT_DXFN12);
	}
	else
	{
		record.reserveNunBytes(4); // cbDxf
		record.reserveNunBytes(2); // reserved
	}
}


void DXFN12::load(CFRecord& record)
{
	record >> cbDxf;
	
	if(cbDxf)
	{
		const size_t end_of_struct_pos = record.getRdPtr() + cbDxf;
		
		dxfn = DXFNPtr(new DXFN);
		record >> *dxfn;
		
		if(record.getRdPtr() < end_of_struct_pos)
		{
			xfext = XFExtNoFRTPtr(new XFExtNoFRT);
			record >> *xfext;
		}
	}
	else
	{
		record.skipNunBytes(2); // reserved
	}
}

int DXFN12::serialize(std::wostream & stream)
{
	if (dxfn && !xfext)
	{
		dxfn->serialize(stream);
	}
	else
	{
		CP_XML_WRITER(stream)    
		{			
			CP_XML_NODE(L"dxf")
			{
				if (xfext)
				{
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

