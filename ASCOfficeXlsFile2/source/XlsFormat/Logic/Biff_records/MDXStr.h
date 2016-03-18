#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of MDXStr record in BIFF8
class MDXStr: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MDXStr)
	BASE_OBJECT_DEFINE_CLASS_NAME(MDXStr)
public:
	MDXStr();
	~MDXStr();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	LPWideString st;

};

} // namespace XLS

