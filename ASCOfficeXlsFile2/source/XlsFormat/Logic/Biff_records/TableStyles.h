#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

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
private:
	BIFF_DWORD cts;
	LPWideStringNoCch rgchDefTableStyle;
	LPWideStringNoCch rgchDefPivotStyle;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(cts)
		BO_ATTRIB_MARKUP_ATTRIB(rgchDefTableStyle)
		BO_ATTRIB_MARKUP_ATTRIB(rgchDefPivotStyle)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

