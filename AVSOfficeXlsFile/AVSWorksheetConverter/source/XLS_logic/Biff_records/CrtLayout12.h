#pragma once

#include "BiffRecord.h"

namespace XLS
{;


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
private:
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

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(dwCheckSum)
		BO_ATTRIB_MARKUP_ATTRIB(autolayouttype)
		BO_ATTRIB_MARKUP_ATTRIB(wXMode)
		BO_ATTRIB_MARKUP_ATTRIB(wYMode)
		BO_ATTRIB_MARKUP_ATTRIB(wWidthMode)
		BO_ATTRIB_MARKUP_ATTRIB(wHeightMode)
		BO_ATTRIB_MARKUP_ATTRIB(x)
		BO_ATTRIB_MARKUP_ATTRIB(y)
		BO_ATTRIB_MARKUP_ATTRIB(dx)
		BO_ATTRIB_MARKUP_ATTRIB(dy)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

