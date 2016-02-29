#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/HorzBrk.h>

namespace XLS
{


// Logical representation of HorizontalPageBreaks record in BIFF8
class HorizontalPageBreaks: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(HorizontalPageBreaks)
	BASE_OBJECT_DEFINE_CLASS_NAME(HorizontalPageBreaks)
public:
	HorizontalPageBreaks();
	~HorizontalPageBreaks();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeHorizontalPageBreaks;


//-----------------------------
	_UINT16 cbrk;
	BiffStructurePtrVector rgbrk;

};

} // namespace XLS

