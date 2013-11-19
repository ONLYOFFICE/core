#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of XCT record in BIFF8
class XCT: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(XCT)
	BASE_OBJECT_DEFINE_CLASS_NAME(XCT)
public:
	XCT();
	~XCT();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD ccrn;
	BIFF_WORD itab;
	BIFF_BOOL itab_exist;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(ccrn)
		if(itab_exist)
		{
			BO_ATTRIB_MARKUP_ATTRIB(itab)
		}
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

