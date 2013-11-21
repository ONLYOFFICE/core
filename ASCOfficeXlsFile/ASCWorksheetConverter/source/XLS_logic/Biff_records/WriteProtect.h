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
public:
	BO_ATTRIB_MARKUP_BEGIN
	BO_ATTRIB_MARKUP_END
};

} // namespace XLS
