#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CalcMode record in BIFF8
class CalcMode: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CalcMode)
	BASE_OBJECT_DEFINE_CLASS_NAME(CalcMode)
public:
	CalcMode();
	~CalcMode();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BSTR fAutoRecalc;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fAutoRecalc)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

