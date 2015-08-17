#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of AutoFilterInfo record in BIFF8
class AutoFilterInfo: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AutoFilterInfo)
	BASE_OBJECT_DEFINE_CLASS_NAME(AutoFilterInfo)
	
	static const ElementType	type = typeAutoFilterInfo;
	
	AutoFilterInfo();
	~AutoFilterInfo();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	BIFF_WORD cEntries;

};

} // namespace XLS

