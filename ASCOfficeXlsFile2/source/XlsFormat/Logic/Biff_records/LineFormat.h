#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/LongRGB.h>

namespace XLS
{


// Logical representation of LineFormat record in BIFF8
class LineFormat: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(LineFormat)
	BASE_OBJECT_DEFINE_CLASS_NAME(LineFormat)
public:
	LineFormat();
	~LineFormat();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	LongRGB rgb;
	BIFF_WORD lns;
	BIFF_SHORT we;
	BIFF_BOOL fAuto;
	BIFF_BOOL fAxisOn;
	BIFF_BOOL fAutoCo;
	IcvChart icv;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_COMPLEX(rgb)
		//BO_ATTRIB_MARKUP_ATTRIB(lns)
		//BO_ATTRIB_MARKUP_ATTRIB(we)
		//BO_ATTRIB_MARKUP_ATTRIB(fAuto)
		//BO_ATTRIB_MARKUP_ATTRIB(fAxisOn)
		//BO_ATTRIB_MARKUP_ATTRIB(fAutoCo)
		//BO_ATTRIB_MARKUP_ATTRIB(icv)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

