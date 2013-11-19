#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Pie record in BIFF8
class Pie: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Pie)
	BASE_OBJECT_DEFINE_CLASS_NAME(Pie)
public:
	Pie();
	~Pie();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD anStart;
	BIFF_WORD pcDonut;
	BIFF_BOOL fHasShadow;
	BIFF_BOOL fShowLdrLines;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(anStart)
		BO_ATTRIB_MARKUP_ATTRIB(pcDonut)
		BO_ATTRIB_MARKUP_ATTRIB(fHasShadow)
		BO_ATTRIB_MARKUP_ATTRIB(fShowLdrLines)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

