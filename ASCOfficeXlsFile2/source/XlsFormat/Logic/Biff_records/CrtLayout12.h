#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of CrtLayout12 record in BIFF8
class CrtLayout12: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(CrtLayout12)
	BASE_OBJECT_DEFINE_CLASS_NAME(CrtLayout12)
public:
	CrtLayout12();
	~CrtLayout12();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeCrtLayout12;

//-----------------------------
	BIFF_DWORD dwCheckSum;
	BIFF_BYTE autolayouttype;
	
	CrtLayout12Mode wXMode;
	CrtLayout12Mode wYMode;
	CrtLayout12Mode wWidthMode;
	CrtLayout12Mode wHeightMode;
	
	BIFF_DOUBLE x;
	BIFF_DOUBLE y;
	BIFF_DOUBLE dx;
	BIFF_DOUBLE dy;
};

} // namespace XLS

