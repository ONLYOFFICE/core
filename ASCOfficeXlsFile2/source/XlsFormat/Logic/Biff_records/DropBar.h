#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of DropBar record in BIFF8
class DropBar: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DropBar)
	BASE_OBJECT_DEFINE_CLASS_NAME(DropBar)
public:
	DropBar();
	~DropBar();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDropBar;

	_INT16 pcGap;
};

} // namespace XLS

