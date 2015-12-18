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

	int serialize(std::wostream & _stream);
//-----------------------------
	LongRGB rgb;
	BIFF_WORD lns;
	BIFF_WORD we;
	BIFF_BOOL fAuto;
	BIFF_BOOL fAxisOn;
	BIFF_BOOL fAutoCo;
	IcvChart icv;	
};

} // namespace XLS

