#include "stdafx.h"
#include "AnyObject.h"
#include <XLS_bin/CFStream.h>


namespace XLS
{;

AnyObject::AnyObject()
{
}


AnyObject::~AnyObject()
{
}

BaseObjectPtr AnyObject::clone()
{
	return BaseObjectPtr(new AnyObject(*this));
}


void AnyObject::writeFields(CFRecord& record)
{
#pragma message("####################### AnyObject record is not implemented")
	Log::info("AnyObject record is not implemented.");
	//record << some_value;
}


void AnyObject::readFields(CFRecord& record)
{
	tag_name = record.getTypeString();
	raw_data_size = record.getDataSize();
	raw_data.reset(new char[raw_data_size]);
	memcpy(raw_data.get(), record.getData(), raw_data_size);
}


} // namespace XLS

