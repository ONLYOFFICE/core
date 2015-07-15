#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of LegendException record in BIFF8
class LegendException: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(LegendException)
	BASE_OBJECT_DEFINE_CLASS_NAME(LegendException)
public:
	LegendException();
	~LegendException();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
//	BIFF_WORD userName;
public:
	//BO_ATTRIB_MARKUP_BEGIN
//		//BO_ATTRIB_MARKUP_ATTRIB(userName)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

