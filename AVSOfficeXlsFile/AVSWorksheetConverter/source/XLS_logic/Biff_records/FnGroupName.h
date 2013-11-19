#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of FnGroupName record in BIFF8
class FnGroupName: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(FnGroupName)
	BASE_OBJECT_DEFINE_CLASS_NAME(FnGroupName)
public:
	FnGroupName();
	~FnGroupName();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	XLUnicodeString rgch;
public:
	BO_ATTRIB_MARKUP_BEGIN
	BO_ATTRIB_MARKUP_ATTRIB(rgch)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

