#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{;


// Logical representation of BigName record in BIFF8
class BigName: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BigName)
	BASE_OBJECT_DEFINE_CLASS_NAME(BigName)
public:
	BigName();
	~BigName();

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

