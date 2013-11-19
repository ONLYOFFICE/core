#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Axis record in BIFF8
class Axis: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Axis)
	BASE_OBJECT_DEFINE_CLASS_NAME(Axis)
public:
	Axis();
	~Axis();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD wType;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(wType)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

