#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CalcDelta record in BIFF8
class CalcDelta: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CalcDelta)
	BASE_OBJECT_DEFINE_CLASS_NAME(CalcDelta)
public:
	CalcDelta();
	~CalcDelta();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_DOUBLE numDelta;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(numDelta)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

