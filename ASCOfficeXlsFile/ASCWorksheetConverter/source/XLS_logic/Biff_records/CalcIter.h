#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CalcIter record in BIFF8
class CalcIter: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CalcIter)
	BASE_OBJECT_DEFINE_CLASS_NAME(CalcIter)
public:
	CalcIter();
	~CalcIter();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Boolean<WORD> vfIter;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(vfIter)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

