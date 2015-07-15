#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of WriteAccess record in BIFF8
class WriteAccess: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(WriteAccess)
	BASE_OBJECT_DEFINE_CLASS_NAME(WriteAccess)
public:
	WriteAccess();
	~WriteAccess();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
	static const ElementType	type = typeWriteAccess;

//-----------------------------
	XLUnicodeString userName;

};

} // namespace XLS

