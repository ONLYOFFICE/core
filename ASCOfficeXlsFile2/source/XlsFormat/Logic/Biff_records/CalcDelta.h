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

	static const ElementType	type = typeCalcDelta;

//-----------------------------
	BIFF_DOUBLE numDelta;

};

} // namespace XLS

