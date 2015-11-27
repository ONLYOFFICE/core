#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FixedPoint.h>

namespace XLS
{


// Logical representation of Chart record in BIFF8
class Chart: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Chart)
	BASE_OBJECT_DEFINE_CLASS_NAME(Chart)
public:
	Chart();
	~Chart();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeChart;

//-----------------------------
	OSHARED::FixedPoint x;
	OSHARED::FixedPoint y;
	OSHARED::FixedPoint dx;
	OSHARED::FixedPoint dy;

};

} // namespace XLS

