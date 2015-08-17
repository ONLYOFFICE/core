#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	static const ElementType	type = typeCalcMode;

//-----------------------------
	BIFF_BSTR fAutoRecalc;

};

} // namespace XLS

