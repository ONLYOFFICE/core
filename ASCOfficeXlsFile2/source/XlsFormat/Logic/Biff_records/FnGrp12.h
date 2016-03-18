#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of FnGrp12 record in BIFF8
class FnGrp12: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(FnGrp12)
	BASE_OBJECT_DEFINE_CLASS_NAME(FnGrp12)
public:
	FnGrp12();
	~FnGrp12();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeFnGrp12;

//-----------------------------
	XLUnicodeString astFnGrp;

};

} // namespace XLS

