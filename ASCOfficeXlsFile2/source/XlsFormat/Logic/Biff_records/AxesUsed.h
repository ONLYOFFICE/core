#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of AxesUsed record in BIFF8
class AxesUsed: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AxesUsed)
	BASE_OBJECT_DEFINE_CLASS_NAME(AxesUsed)
public:
	AxesUsed();
	~AxesUsed();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeAxesUsed;

//-----------------------------
	BIFF_WORD cAxes;

};

} // namespace XLS

