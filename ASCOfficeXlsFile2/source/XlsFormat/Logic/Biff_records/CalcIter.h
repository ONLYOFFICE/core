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

	static const ElementType	type = typeCalcIter;

//-----------------------------
	Boolean<unsigned __int16> vfIter;

};

} // namespace XLS

