#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of LPr record in BIFF8
class LPr: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(LPr)
	BASE_OBJECT_DEFINE_CLASS_NAME(LPr)
public:
	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
public:
	BO_ATTRIB_MARKUP_BEGIN
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

