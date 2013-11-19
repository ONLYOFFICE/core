#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/LongRGB.h>

namespace XLS
{;


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
private:
	LongRGB rgbFore;
	LongRGB rgbBack;
	BIFF_WORD fls;
	BIFF_BOOL fAuto;
	BIFF_BOOL fInvertNeg;
	IcvChart icvFore;
	IcvChart icvBack;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(rgbFore)
		BO_ATTRIB_MARKUP_COMPLEX(rgbBack)
		BO_ATTRIB_MARKUP_ATTRIB(fls)
		BO_ATTRIB_MARKUP_ATTRIB(fAuto)
		BO_ATTRIB_MARKUP_ATTRIB(fInvertNeg)
		BO_ATTRIB_MARKUP_ATTRIB(icvFore)
		BO_ATTRIB_MARKUP_ATTRIB(icvBack)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

