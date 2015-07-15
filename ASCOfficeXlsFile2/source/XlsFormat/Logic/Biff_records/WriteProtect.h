#pragma once

#include "BiffRecord.h"

namespace XLS
{;

class CFStreamCacheReader;


// Logical representation of WriteProtect record in BIFF8
class WriteProtect: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(WriteProtect)
	BASE_OBJECT_DEFINE_CLASS_NAME(WriteProtect)
public:
	WriteProtect();
	~WriteProtect();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeWriteProtect;

};

} // namespace XLS
