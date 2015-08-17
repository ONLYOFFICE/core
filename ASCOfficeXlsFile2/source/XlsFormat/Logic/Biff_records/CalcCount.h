#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	static const ElementType	type = typeCalcCount;

//-----------------------------
	BIFF_SHORT cIter;
};

} // namespace XLS

