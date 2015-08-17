#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	static const ElementType	type = typeUserSViewEnd;


};

} // namespace XLS

