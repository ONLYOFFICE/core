#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Date1904 record in BIFF8
class Date1904: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Date1904)
	BASE_OBJECT_DEFINE_CLASS_NAME(Date1904)
public:
	Date1904();
	~Date1904();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	Boolean<unsigned __int16> f1904DateSystem;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(f1904DateSystem)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

