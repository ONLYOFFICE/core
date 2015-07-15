#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CalcSaveRecalc record in BIFF8
class CalcSaveRecalc: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CalcSaveRecalc)
	BASE_OBJECT_DEFINE_CLASS_NAME(CalcSaveRecalc)
public:
	CalcSaveRecalc();
	~CalcSaveRecalc();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCalcSaveRecalc;

//-----------------------------
	Boolean<unsigned __int16> fSaveRecalc;

	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(fSaveRecalc)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

