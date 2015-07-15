#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CalcRefMode record in BIFF8
class CalcRefMode: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CalcRefMode)
	BASE_OBJECT_DEFINE_CLASS_NAME(CalcRefMode)
public:
	CalcRefMode();
	~CalcRefMode();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCalcRefMode;

//-----------------------------
	BIFF_BSTR fRefA1;

	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(fRefA1)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

