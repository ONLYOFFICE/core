#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/LongRGB.h>

namespace XLS
{


// Logical representation of AreaFormat record in BIFF8
class AreaFormat: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AreaFormat)
	BASE_OBJECT_DEFINE_CLASS_NAME(AreaFormat)
public:
	AreaFormat();
	~AreaFormat();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
		
	static const ElementType	type = typeAreaFormat;

	LongRGB rgbFore;
	LongRGB rgbBack;
	BIFF_WORD fls;
	BIFF_BOOL fAuto;
	BIFF_BOOL fInvertNeg;
	IcvChart icvFore;
	IcvChart icvBack;


};

} // namespace XLS

