#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of DataFormat record in BIFF8
class DataFormat: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DataFormat)
	BASE_OBJECT_DEFINE_CLASS_NAME(DataFormat)
public:
	DataFormat();
	~DataFormat();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD xi;
	BIFF_WORD yi;
	BIFF_WORD iss;
	BIFF_BOOL fUnknown;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(xi)
		BO_ATTRIB_MARKUP_ATTRIB(yi)
		BO_ATTRIB_MARKUP_ATTRIB(iss)
		BO_ATTRIB_MARKUP_ATTRIB(fUnknown)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

