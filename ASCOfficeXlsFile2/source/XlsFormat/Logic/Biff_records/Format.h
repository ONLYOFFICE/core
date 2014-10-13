#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of Format record in BIFF8
class Format: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Format)
	BASE_OBJECT_DEFINE_CLASS_NAME(Format)
public:
	Format();
	~Format();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD ifmt;
	XLUnicodeString stFormat;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(ifmt)
		BO_ATTRIB_MARKUP_ATTRIB(stFormat)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

