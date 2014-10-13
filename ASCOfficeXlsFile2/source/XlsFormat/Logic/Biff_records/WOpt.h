#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of WOpt record in BIFF8
class WOpt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(WOpt)
	BASE_OBJECT_DEFINE_CLASS_NAME(WOpt)
public:
	WOpt();
	~WOpt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
//	BIFF_WORD userName;
public:
	BO_ATTRIB_MARKUP_BEGIN
//		BO_ATTRIB_MARKUP_ATTRIB(userName)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

