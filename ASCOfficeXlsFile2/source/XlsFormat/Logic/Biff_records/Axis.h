#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Axis record in BIFF8
class Axis: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Axis)
	BASE_OBJECT_DEFINE_CLASS_NAME(Axis)
public:
	Axis();
	~Axis();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeAxis;;

//-----------------------------
	BIFF_WORD wType;

};

} // namespace XLS

