#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of UserSViewEnd record in BIFF8
class UserSViewEnd: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(UserSViewEnd)
	BASE_OBJECT_DEFINE_CLASS_NAME(UserSViewEnd)
public:
	UserSViewEnd();
	~UserSViewEnd();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
public:
	BO_ATTRIB_MARKUP_BEGIN
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

