#pragma once

#include "BiffRecord.h"
#include "../../Logic/Biff_structures/FixedPoint.h"

namespace XLS
{;


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

public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(x)
		//BO_ATTRIB_MARKUP_ATTRIB(y)
		//BO_ATTRIB_MARKUP_ATTRIB(dx)
		//BO_ATTRIB_MARKUP_ATTRIB(dy)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

