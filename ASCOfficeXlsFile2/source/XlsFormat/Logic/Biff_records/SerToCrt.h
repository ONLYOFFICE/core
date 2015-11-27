#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of SerToCrt record in BIFF8
class SerToCrt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SerToCrt)
	BASE_OBJECT_DEFINE_CLASS_NAME(SerToCrt)
public:
	SerToCrt();
	~SerToCrt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSerToCrt;

//-----------------------------
	BIFF_WORD id;

};

} // namespace XLS

