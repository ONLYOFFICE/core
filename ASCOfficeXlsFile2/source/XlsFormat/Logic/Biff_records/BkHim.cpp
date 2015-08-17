
#include "BkHim.h"

namespace XLS
{

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
	
	unsigned short cf;
	unsigned int lcb;

	record >> cf;
	record.skipNunBytes(2);
	record >> lcb;//imageBlob size

	unsigned int sz = record.getDataSize() - record.getRdPtr();

	if (cf == 0x09)
	{
		//bitmap
	}
	else
	{
		//native
	}
	
	//imageBlob
	if (record.checkFitReadSafe(lcb))
	{

		record.skipNunBytes(lcb);
	}
}

} // namespace XLS

