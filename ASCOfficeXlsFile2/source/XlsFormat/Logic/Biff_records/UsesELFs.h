#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of UsesELFs record in BIFF8
class UsesELFs: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(UsesELFs)
	BASE_OBJECT_DEFINE_CLASS_NAME(UsesELFs)
public:
	UsesELFs();
	~UsesELFs();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeUsesELFs;

//-----------------------------
	Boolean<unsigned short> useselfs;

};

} // namespace XLS

