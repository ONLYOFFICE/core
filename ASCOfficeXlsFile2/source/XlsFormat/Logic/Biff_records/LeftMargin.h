#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of LeftMargin record in BIFF8
class LeftMargin: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(LeftMargin)
	BASE_OBJECT_DEFINE_CLASS_NAME(LeftMargin)
public:
	LeftMargin();
	~LeftMargin();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	BIFF_DOUBLE num;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(num)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

