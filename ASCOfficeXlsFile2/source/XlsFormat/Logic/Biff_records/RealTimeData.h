#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{


// Logical representation of RealTimeData record in BIFF8
class RealTimeData: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(RealTimeData)
	BASE_OBJECT_DEFINE_CLASS_NAME(RealTimeData)
public:
	RealTimeData();
	~RealTimeData();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeRealTimeData;

};

} // namespace XLS

