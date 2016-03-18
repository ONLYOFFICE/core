#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	static const ElementType	type = typeDate1904;

//-----------------------------
	Boolean<unsigned short> f1904DateSystem;

};

} // namespace XLS

