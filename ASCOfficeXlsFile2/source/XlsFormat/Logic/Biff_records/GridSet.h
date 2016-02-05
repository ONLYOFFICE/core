#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of GridSet record in BIFF8
class GridSet: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(GridSet)
	BASE_OBJECT_DEFINE_CLASS_NAME(GridSet)
public:
	GridSet();
	~GridSet();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeGridSet;
};

} // namespace XLS

