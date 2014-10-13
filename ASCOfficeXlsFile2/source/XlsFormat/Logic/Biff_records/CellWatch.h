#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FrtRefHeaderU.h>

namespace XLS
{;


// Logical representation of CellWatch record in BIFF8
class CellWatch: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CellWatch)
	BASE_OBJECT_DEFINE_CLASS_NAME(CellWatch)
public:
	CellWatch();
	~CellWatch();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	FrtRefHeaderU frtRefHeaderU;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(frtRefHeaderU)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

