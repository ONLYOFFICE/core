#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Excel9File record in BIFF8
class Excel9File: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Excel9File)
	BASE_OBJECT_DEFINE_CLASS_NAME(Excel9File)
public:
	Excel9File();
	~Excel9File();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
//	XLUnicodeString userName;
public:
	BO_ATTRIB_MARKUP_BEGIN
//	BO_ATTRIB_MARKUP_ATTRIB(userName)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

