#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of AutoFilterInfo record in BIFF8
class AutoFilterInfo: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AutoFilterInfo)
	BASE_OBJECT_DEFINE_CLASS_NAME(AutoFilterInfo)
public:
	AutoFilterInfo();
	~AutoFilterInfo();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD cEntries;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(cEntries)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

