#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	static const ElementType	type = typeEndObject;

//-----------------------------
	_UINT16 iObjectKind;

};

} // namespace XLS

