#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of PrintSize record in BIFF8
class PrintSize: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(PrintSize)
	BASE_OBJECT_DEFINE_CLASS_NAME(PrintSize)
public:
	PrintSize();
	~PrintSize();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD printSize;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(printSize)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

