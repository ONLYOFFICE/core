#include "stdafx.h"
#include "BkHim.h"

namespace XLS
{;

BkHim::BkHim()
{
}


BkHim::~BkHim()
{
}


BaseObjectPtr BkHim::clone()
{
	return BaseObjectPtr(new BkHim(*this));
}


void BkHim::writeFields(CFRecord& record)
{
#pragma message("####################### BkHim record is not implemented")
	Log::error("BkHim record is not implemented.");
	//record << some_value;
}


void BkHim::readFields(CFRecord& record)
{
//#pragma message("####################### BkHim record is not implemented")
	//Log::error("BkHim record is not implemented.");
	//record >> some_value;
	
	WORD cf;
	DWORD lcb;

	record >> cf;
	record.skipNBytes(2);
	record >> lcb;
	record.skipNBytes(lcb);
}

} // namespace XLS

