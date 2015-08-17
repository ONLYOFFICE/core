#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


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

	static const ElementType	type = typeFormat;

	int serialize(std::wostream & stream);
//-----------------------------
	BIFF_WORD ifmt;
	XLUnicodeString stFormat;

};

} // namespace XLS

