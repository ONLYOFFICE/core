#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Password record in BIFF8
class Password: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Password)
	BASE_OBJECT_DEFINE_CLASS_NAME(Password)
public:
	Password();
	~Password();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BSTR wPassword;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(wPassword)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

