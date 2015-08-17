#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{

// Logical representation of TableStyles record in BIFF8
class TableStyles: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(TableStyles)
	BASE_OBJECT_DEFINE_CLASS_NAME(TableStyles)
public:
	TableStyles();
	~TableStyles();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeTableStyles;

//-----------------------------
	BIFF_DWORD cts;
	LPWideStringNoCch rgchDefTableStyle;
	LPWideStringNoCch rgchDefPivotStyle;

};

} // namespace XLS

