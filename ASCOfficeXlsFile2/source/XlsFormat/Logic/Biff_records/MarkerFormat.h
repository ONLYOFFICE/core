#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/LongRGB.h>

namespace XLS
{;


// Logical representation of MarkerFormat record in BIFF8
class MarkerFormat: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MarkerFormat)
	BASE_OBJECT_DEFINE_CLASS_NAME(MarkerFormat)
public:
	MarkerFormat();
	~MarkerFormat();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	LongRGB rgbFore;
	LongRGB rgbBack;
	BIFF_WORD imk;
	BIFF_BOOL fAuto;
	BIFF_BOOL fNotShowInt;
	BIFF_BOOL fNotShowBrd;
	IcvChart icvFore;
	IcvChart icvBack;
	BIFF_DWORD miSize;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_COMPLEX(rgbFore)
		//BO_ATTRIB_MARKUP_COMPLEX(rgbBack)
		//BO_ATTRIB_MARKUP_ATTRIB(imk)
		//BO_ATTRIB_MARKUP_ATTRIB(fAuto)
		//BO_ATTRIB_MARKUP_ATTRIB(fNotShowInt)
		//BO_ATTRIB_MARKUP_ATTRIB(fNotShowBrd)
		//BO_ATTRIB_MARKUP_ATTRIB(icvFore)
		//BO_ATTRIB_MARKUP_ATTRIB(icvBack)
		//BO_ATTRIB_MARKUP_ATTRIB(miSize)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

