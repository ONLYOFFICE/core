#include "stdafx.h"
#include "FtCf.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FtCf::clone()
{
	return BiffStructurePtr(new FtCf(*this));
}


void FtCf::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	own_tag->setAttribute(L"cf", cf);
}


void FtCf::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	cf = getStructAttribute(own_tag, L"cf");
}


void FtCf::store(CFRecord& record)
{
	WORD ft = 0x07; // reserved
	WORD cb = 0x02; // reserved
	record << ft << cb;

	record << cf;
}


void FtCf::load(CFRecord& record)
{
	//record.skipNBytes(4); // reserved
	WORD ft, cb;
	record >> ft;
	
	// OpenOffice fuck!!!
	if (ft == 0)
		record.RollRdPtrBack(2);		
	else
	{
		record >> cb;
		record >> cf;
	}
}


} // namespace XLS

