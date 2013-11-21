#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of SXViewEx9 record in BIFF8
class SXViewEx9: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SXViewEx9)
	BASE_OBJECT_DEFINE_CLASS_NAME(SXViewEx9)
public:
	SXViewEx9();
	~SXViewEx9();

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

