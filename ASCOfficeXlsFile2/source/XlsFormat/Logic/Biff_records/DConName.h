#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of DConName record in BIFF8
class DConName: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DConName)
	BASE_OBJECT_DEFINE_CLASS_NAME(DConName)
public:
	DConName();
	~DConName();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeDConName;

};

} // namespace XLS

