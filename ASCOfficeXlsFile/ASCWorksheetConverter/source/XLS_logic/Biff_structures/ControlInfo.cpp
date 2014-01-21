#include "stdafx.h"
#include "ControlInfo.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr ControlInfo::clone()
{
	return BiffStructurePtr(new ControlInfo(*this));
}

void ControlInfo::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"fDefault", fDefault);
	xml_tag->setAttribute(L"fHelp", fHelp);
	xml_tag->setAttribute(L"fCancel", fCancel);
	xml_tag->setAttribute(L"fDismiss", fDismiss);
	xml_tag->setAttribute(L"accel1", accel1);
}

void ControlInfo::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
}

void ControlInfo::store(CFRecord& record)
{	
}

void ControlInfo::load(CFRecord& record)
{
	WORD flags;
	record >> flags;

	fDefault = GETBIT(flags, 0);
	fHelp = GETBIT(flags, 1);
	fCancel = GETBIT(flags, 2);
	fDismiss = GETBIT(flags, 3);

	record >> accel1;
	record.skipNBytes(2);	// reserved2
}


} // namespace XLS

