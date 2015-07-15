#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Units record in BIFF8
class Units: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Units)
	BASE_OBJECT_DEFINE_CLASS_NAME(Units)
public:
	Units();
	~Units();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeUnits;

};

} // namespace XLS

