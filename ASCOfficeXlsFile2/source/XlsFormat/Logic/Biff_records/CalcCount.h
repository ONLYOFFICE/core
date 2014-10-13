#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CalcCount record in BIFF8
class CalcCount: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CalcCount)
	BASE_OBJECT_DEFINE_CLASS_NAME(CalcCount)
public:
	CalcCount();
	~CalcCount();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_SHORT cIter;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(cIter)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

