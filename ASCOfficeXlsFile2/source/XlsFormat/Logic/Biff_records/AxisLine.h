#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of AxisLine record in BIFF8
class AxisLine: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AxisLine)
	BASE_OBJECT_DEFINE_CLASS_NAME(AxisLine)
public:
	AxisLine();
	~AxisLine();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD id;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(id)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

