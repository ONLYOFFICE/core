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
	BIFF_WORD iObjectKind;
	BIFF_WORD iObjectContext;
	BIFF_WORD iObjectInstance1;
	BIFF_WORD iObjectInstance2;


};

} // namespace XLS

