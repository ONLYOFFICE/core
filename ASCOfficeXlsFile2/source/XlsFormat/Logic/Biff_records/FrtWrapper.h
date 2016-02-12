#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of FrtWrapper record in BIFF8
class FrtWrapper: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(FrtWrapper)
	BASE_OBJECT_DEFINE_CLASS_NAME(FrtWrapper)
public:
	FrtWrapper();
	~FrtWrapper();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFrtWrapper;
};

} // namespace XLS

