#include "stdafx.h"
#include "ObjLinkFmla.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


ObjLinkFmla::ObjLinkFmla()
:	fmla(false)
{
}

BiffStructurePtr ObjLinkFmla::clone()
{
	return BiffStructurePtr(new ObjLinkFmla(*this));
}


void ObjLinkFmla::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	fmla.toXML(own_tag);
}


void ObjLinkFmla::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	fmla.fromXML(own_tag);
}


void ObjLinkFmla::store(CFRecord& record, const WORD ot)
{
	WORD ft = ot == 0x0B || ot == 0x0C ? 0x0014 : 0x000E; // reserved
	record << ft;

	fmla.store(record);
}


void ObjLinkFmla::load(CFRecord& record)
{
	record.skipNBytes(2); // reserved

	fmla.load(record);
}


} // namespace XLS

