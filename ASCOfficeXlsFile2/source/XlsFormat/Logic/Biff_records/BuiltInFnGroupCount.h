#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of BuiltInFnGroupCount record in BIFF8
class BuiltInFnGroupCount: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BuiltInFnGroupCount)
	BASE_OBJECT_DEFINE_CLASS_NAME(BuiltInFnGroupCount)
public:
	BuiltInFnGroupCount();
	~BuiltInFnGroupCount();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeBuiltInFnGroupCount;


//-----------------------------
	BIFF_WORD count;
};

} // namespace XLS

