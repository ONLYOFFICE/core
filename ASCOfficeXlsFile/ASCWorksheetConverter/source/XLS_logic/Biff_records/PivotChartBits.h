#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of PivotChartBits record in BIFF8
class PivotChartBits: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(PivotChartBits)
	BASE_OBJECT_DEFINE_CLASS_NAME(PivotChartBits)
public:
	PivotChartBits();
	~PivotChartBits();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
//	BIFF_WORD userName;
public:
	BO_ATTRIB_MARKUP_BEGIN
//		BO_ATTRIB_MARKUP_ATTRIB(userName)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

