#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/LongRGB.h>

namespace XLS
{;


// Logical representation of Tick record in BIFF8
class Tick: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Tick)
	BASE_OBJECT_DEFINE_CLASS_NAME(Tick)
public:
	Tick();
	~Tick();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BYTE tktMajor;
	BIFF_BYTE tktMinor;
	BIFF_BYTE tlt;
	BIFF_BYTE wBkgMode;
	LongRGB rgb;

	BIFF_BOOL fAutoCo;
	BIFF_BOOL fAutoMode;
	BIFF_BYTE rot;
	BIFF_BOOL fAutoRot;
	BIFF_BYTE iReadingOrder;

	Icv icv;
	BIFF_WORD trot;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(tktMajor)
		BO_ATTRIB_MARKUP_ATTRIB(tktMinor)
		BO_ATTRIB_MARKUP_ATTRIB(tlt)
		BO_ATTRIB_MARKUP_ATTRIB(wBkgMode)
		BO_ATTRIB_MARKUP_COMPLEX(rgb)

		BO_ATTRIB_MARKUP_ATTRIB(fAutoCo)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoMode)
		BO_ATTRIB_MARKUP_ATTRIB(rot)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoRot)
		BO_ATTRIB_MARKUP_ATTRIB(iReadingOrder)

		BO_ATTRIB_MARKUP_ATTRIB(icv)
		BO_ATTRIB_MARKUP_ATTRIB(trot)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

