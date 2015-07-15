#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of AxisParent record in BIFF8
class AxisParent: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AxisParent)
	BASE_OBJECT_DEFINE_CLASS_NAME(AxisParent)
public:
	AxisParent();
	~AxisParent();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeAxisParent;

//-----------------------------
	Boolean<unsigned __int16> iax;

};

} // namespace XLS

