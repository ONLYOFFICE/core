#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of StartObject record in BIFF8
class StartObject: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(StartObject)
	BASE_OBJECT_DEFINE_CLASS_NAME(StartObject)
public:
	StartObject();
	~StartObject();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD iObjectKind;
	BIFF_WORD iObjectContext;
	BIFF_WORD iObjectInstance1;
	BIFF_WORD iObjectInstance2;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(iObjectKind)
		BO_ATTRIB_MARKUP_ATTRIB(iObjectContext)
		BO_ATTRIB_MARKUP_ATTRIB(iObjectInstance1)
		BO_ATTRIB_MARKUP_ATTRIB(iObjectInstance2)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

