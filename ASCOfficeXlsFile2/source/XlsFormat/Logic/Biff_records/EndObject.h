#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of EndObject record in BIFF8
class EndObject: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(EndObject)
	BASE_OBJECT_DEFINE_CLASS_NAME(EndObject)
public:
	EndObject();
	~EndObject();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_WORD iObjectKind;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(iObjectKind)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

