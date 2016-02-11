#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{


// Logical representation of RRSort record in BIFF8
class RRSort: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(RRSort)
	BASE_OBJECT_DEFINE_CLASS_NAME(RRSort)
public:
	RRSort();
	~RRSort();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeRRSort;


};

} // namespace XLS

