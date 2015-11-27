#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Series record in BIFF8
class Series: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Series)
	BASE_OBJECT_DEFINE_CLASS_NAME(Series)
public:
	Series();
	~Series();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSeries;

//-----------------------------
	BIFF_WORD sdtX;
	BIFF_WORD cValx;
	BIFF_WORD cValy;
	BIFF_WORD cValBSize;
};

} // namespace XLS

