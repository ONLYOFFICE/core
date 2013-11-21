#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Dat record in BIFF8
class Dat: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Dat)
	BASE_OBJECT_DEFINE_CLASS_NAME(Dat)
public:
	Dat();
	~Dat();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

private:
	BIFF_BOOL fHasBordHorz;
	BIFF_BOOL fHasBordVert;
	BIFF_BOOL fHasBordOutline;
	BIFF_BOOL fShowSeriesKey;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fHasBordHorz)
		BO_ATTRIB_MARKUP_ATTRIB(fHasBordVert)
		BO_ATTRIB_MARKUP_ATTRIB(fHasBordOutline)
		BO_ATTRIB_MARKUP_ATTRIB(fShowSeriesKey)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

