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

	static const ElementType	type = typeValueRange;

//-----------------------------
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
};

} // namespace XLS

