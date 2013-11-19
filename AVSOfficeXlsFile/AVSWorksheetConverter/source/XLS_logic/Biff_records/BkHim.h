#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{;


// Logical representation of BkHim record in BIFF8
class BkHim: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BkHim)
	BASE_OBJECT_DEFINE_CLASS_NAME(BkHim)
public:
	BkHim();
	~BkHim();

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

