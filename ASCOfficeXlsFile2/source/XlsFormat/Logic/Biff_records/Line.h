#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Line record in BIFF8
class Line: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Line)
	BASE_OBJECT_DEFINE_CLASS_NAME(Line)
public:
	Line();
	~Line();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BOOL fStacked;
	BIFF_BOOL f100;
	BIFF_BOOL fHasShadow;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fStacked)
		BO_ATTRIB_MARKUP_ATTRIB(f100)
		BO_ATTRIB_MARKUP_ATTRIB(fHasShadow)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

