#include "stdafx.h"
#include "DXFN12.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr DXFN12::clone()
{
	return BiffStructurePtr(new DXFN12(*this));
}


void DXFN12::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	if(cbDxf)
	{
		dxfn->toXML(xml_tag);
		if(xfext)
		{
			xfext->toXML(xml_tag);
		}
	}
}


void DXFN12::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	DXFNPtr dxfn_temp(new DXFN);
	if(dxfn_temp->fromXML(xml_tag))
	{
		std::swap(dxfn, dxfn_temp);
		XFExtNoFRTPtr xfext_temp(new XFExtNoFRT);
		if(xfext_temp->fromXML(xml_tag))
		{
			std::swap(xfext, xfext_temp);
		}
	}
}


void DXFN12::store(CFRecord& record)
{
	if(dxfn)
	{
		record.registerDelayedDataReceiver(NULL, sizeof(DWORD)/*cbDxf*/, rt_STRUCT_DXFN12);
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
		record.reserveNBytes(4); // cbDxf
		record.reserveNBytes(2); // reserved
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
		record.skipNBytes(2); // reserved
	}
}


} // namespace XLS

