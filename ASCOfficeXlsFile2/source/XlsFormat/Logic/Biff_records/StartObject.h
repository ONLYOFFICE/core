#pragma once

#include "BiffRecord.h"

namespace XLS
{


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

	static const ElementType	type = typeStartObject;

//-----------------------------
	_UINT16 iObjectKind;
	_UINT16 iObjectContext;
	_UINT16 iObjectInstance1;
	_UINT16 iObjectInstance2;


};

} // namespace XLS

