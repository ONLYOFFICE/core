#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of SXTbl record in BIFF8
class SXTbl: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXTbl)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXTbl)
public:
	SXTbl();
	~SXTbl();

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

