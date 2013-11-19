#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{;


// Logical representation of SxItm record in BIFF8
class SxItm: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SxItm)
	BASE_OBJECT_DEFINE_CLASS_NAME(SxItm)
public:
	SxItm();
	~SxItm();

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

