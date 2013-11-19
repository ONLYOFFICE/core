#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Series record in BIFF8
class Series: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Series)
	BASE_OBJECT_DEFINE_CLASS_NAME(Series)
public:
	Series();
	~Series();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD sdtX;
	BIFF_WORD cValx;
	BIFF_WORD cValy;
	BIFF_WORD cValBSize;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(sdtX)
		BO_ATTRIB_MARKUP_ATTRIB(cValx)
		BO_ATTRIB_MARKUP_ATTRIB(cValy)
		BO_ATTRIB_MARKUP_ATTRIB(cValBSize)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

