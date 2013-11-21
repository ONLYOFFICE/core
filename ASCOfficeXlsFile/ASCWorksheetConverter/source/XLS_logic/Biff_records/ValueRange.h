#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ValueRange record in BIFF8
class ValueRange: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ValueRange)
	BASE_OBJECT_DEFINE_CLASS_NAME(ValueRange)
public:
	ValueRange();
	~ValueRange();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_DOUBLE numMin;
	BIFF_DOUBLE numMax;
	BIFF_DOUBLE numMajor;
	BIFF_DOUBLE numMinor;
	BIFF_DOUBLE numCross;

	BIFF_BOOL fAutoMin;
	BIFF_BOOL fAutoMax;
	BIFF_BOOL fAutoMajor;
	BIFF_BOOL fAutoMinor;
	BIFF_BOOL fAutoCross;
	BIFF_BOOL fLog;
	BIFF_BOOL fReversed;
	BIFF_BOOL fMaxCross;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(numMin)
		BO_ATTRIB_MARKUP_ATTRIB(numMax)
		BO_ATTRIB_MARKUP_ATTRIB(numMajor)
		BO_ATTRIB_MARKUP_ATTRIB(numMinor)
		BO_ATTRIB_MARKUP_ATTRIB(numCross)
	
		BO_ATTRIB_MARKUP_ATTRIB(fAutoMin)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoMax)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoMajor)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoMinor)
		BO_ATTRIB_MARKUP_ATTRIB(fAutoCross)
		BO_ATTRIB_MARKUP_ATTRIB(fLog)
		BO_ATTRIB_MARKUP_ATTRIB(fReversed)
		BO_ATTRIB_MARKUP_ATTRIB(fMaxCross)

	BO_ATTRIB_MARKUP_END
};

} // namespace XLS

