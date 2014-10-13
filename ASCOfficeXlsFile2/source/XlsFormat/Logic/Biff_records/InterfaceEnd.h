#pragma once

#include "BiffRecord.h"

namespace XLS
{;

// Logical representation of InterfaceEnd record in BIFF8
class InterfaceEnd: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(InterfaceEnd)
	BASE_OBJECT_DEFINE_CLASS_NAME(InterfaceEnd)
public:
	InterfaceEnd();
	~InterfaceEnd();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	BO_ATTRIB_MARKUP_BEGIN
	BO_ATTRIB_MARKUP_END
};

} // namespace XLS

