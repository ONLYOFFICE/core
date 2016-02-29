#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/LongRGB.h>

namespace XLS
{


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

	int serialize(std::wostream & _stream);
//-----------------------------
	LongRGB rgbFore;
	LongRGB rgbBack;
	
	_UINT16 imk;
	bool fAuto;
	bool fNotShowInt;
	bool fNotShowBrd;
	
	IcvChart icvFore;
	IcvChart icvBack;
	BIFF_DWORD miSize;
};

} // namespace XLS

