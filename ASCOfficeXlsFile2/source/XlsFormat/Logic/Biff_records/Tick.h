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

	int serialize(std::wostream & _stream);

//-----------------------------
	unsigned char	tktMajor;
	unsigned char	tktMinor;
	unsigned char	tlt;
	unsigned char	wBkgMode;
	LongRGB rgb;

	bool fAutoCo;
	bool fAutoMode;
	unsigned char	rot;
	bool fAutoRot;
	unsigned char	iReadingOrder;

	_UINT16 icv;
	_UINT16 trot;

	int _rott;
};

} // namespace XLS

