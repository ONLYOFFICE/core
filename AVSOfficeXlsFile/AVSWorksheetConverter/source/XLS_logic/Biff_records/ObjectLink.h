#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of ObjectLink record in BIFF8
class ObjectLink: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ObjectLink)
	BASE_OBJECT_DEFINE_CLASS_NAME(ObjectLink)
public:
	ObjectLink();
	~ObjectLink();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD wLinkObj;
	BIFF_WORD wLinkVar1;
	BIFF_WORD wLinkVar2;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(wLinkObj)
		BO_ATTRIB_MARKUP_ATTRIB(wLinkVar1)
		BO_ATTRIB_MARKUP_ATTRIB(wLinkVar2)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

