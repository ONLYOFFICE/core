#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of Scl record in BIFF8
class Scl: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Scl)
	BASE_OBJECT_DEFINE_CLASS_NAME(Scl)
public:
	Scl();
	~Scl();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeScl;

//-----------------------------
	_INT16 nscl;
	_INT16 dscl;

};

} // namespace XLS

