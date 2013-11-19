#include "stdafx.h"
#include "FtGboData.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FtGboData::clone()
{
	return BiffStructurePtr(new FtGboData(*this));
}


void FtGboData::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	own_tag->setAttribute(L"accel", accel);
	own_tag->setAttribute(L"fNo3d", fNo3d);
}


void FtGboData::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	accel = getStructAttribute(own_tag, L"accel");
	fNo3d = getStructAttribute(own_tag, L"fNo3d");
}


void FtGboData::store(CFRecord& record)
{
	WORD ft = 0x000F; // reserved
	WORD cb = 0x0006; // reserved
	record << ft << cb;

	record << accel;
	record.reserveNBytes(2); // reserved
	WORD flags = 0;
	SETBIT(flags, 0, fNo3d);
	record << flags;
}


void FtGboData::load(CFRecord& record)
{
	record.skipNBytes(4); // reserved

	record >> accel;
	record.skipNBytes(2); // reserved
	WORD flags;
	record >> flags;
	fNo3d = GETBIT(flags, 0);
}


} // namespace XLS

