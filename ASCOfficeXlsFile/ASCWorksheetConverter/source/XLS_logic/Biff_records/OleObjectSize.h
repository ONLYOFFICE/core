#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/CellRangeRef.h>

namespace XLS
{;


// Logical representation of OleObjectSize record in BIFF8
class OleObjectSize: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(OleObjectSize)
	BASE_OBJECT_DEFINE_CLASS_NAME(OleObjectSize)
public:
	OleObjectSize();
	~OleObjectSize();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	RefU ref_;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(ref_)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

