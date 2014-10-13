#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of BottomMargin record in BIFF8
class BottomMargin: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BottomMargin)
	BASE_OBJECT_DEFINE_CLASS_NAME(BottomMargin)
public:
	BottomMargin();
	~BottomMargin();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_DOUBLE num;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(num)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

