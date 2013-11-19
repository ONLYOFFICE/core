#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of DropBar record in BIFF8
class DropBar: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DropBar)
	BASE_OBJECT_DEFINE_CLASS_NAME(DropBar)
public:
	DropBar();
	~DropBar();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_SHORT pcGap;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(pcGap)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

