#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of CalcPrecision record in BIFF8
class CalcPrecision: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CalcPrecision)
	BASE_OBJECT_DEFINE_CLASS_NAME(CalcPrecision)
public:
	CalcPrecision();
	~CalcPrecision();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCalcPrecision;

//-----------------------------
	Boolean<unsigned short> fFullPrec;

	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(fFullPrec)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

