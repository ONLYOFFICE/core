#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/LongRGB.h>

namespace XLS
{


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

	static const ElementType	type = typeTick;

//-----------------------------
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

};

} // namespace XLS

