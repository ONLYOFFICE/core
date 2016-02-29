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
	_UINT16 sdtX;
	_UINT16 sdtY;
	_UINT16 sdtBSize;

	_UINT16 cValx;
	_UINT16 cValy;
	_UINT16 cValBSize;
};

} // namespace XLS

