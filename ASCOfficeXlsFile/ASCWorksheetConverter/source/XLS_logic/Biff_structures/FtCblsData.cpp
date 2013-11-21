#include "stdafx.h"
#include "FtCblsData.h"
#include <XLS_bin/CFRecord.h>
#include <Exception/AttributeDataWrong.h>

namespace XLS
{;


BiffStructurePtr FtCblsData::clone()
{
	return BiffStructurePtr(new FtCblsData(*this));
}


void FtCblsData::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	own_tag->setAttribute(L"fChecked", fChecked);
	own_tag->setAttribute(L"accel", accel);
	own_tag->setAttribute(L"fNo3d", fNo3d);
}


void FtCblsData::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	fChecked = getStructAttribute(own_tag, L"fChecked");
	accel = getStructAttribute(own_tag, L"accel");
	fNo3d = getStructAttribute(own_tag, L"fNo3d");
}


void FtCblsData::store(CFRecord& record)
{
	WORD ft = 0x0012; // reserved
	WORD cb = 0x0008; // reserved
	record << ft << cb;

	record << fChecked << accel;
	
	record.reserveNBytes(2); // reserved
	WORD flags = 0;
	SETBIT(flags, 0, fNo3d);

	record << flags;
}


void FtCblsData::load(CFRecord& record)
{
	record.skipNBytes(4); // reserved

	record >> fChecked >> accel;
	record.skipNBytes(2); // reserved
	WORD flags;
	record >> flags;
	fNo3d = GETBIT(flags, 0);

}


} // namespace XLS

