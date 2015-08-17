#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of BoundSheet8 record in BIFF8
class BoundSheet8: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BoundSheet8)
	BASE_OBJECT_DEFINE_CLASS_NAME(BoundSheet8)
public:
	BoundSheet8();
	~BoundSheet8();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeBoundSheet8;

//-----------------------------
	ForwardOnlyParam<unsigned int> lbPlyPos;
	BIFF_BSTR hsState;
	BIFF_BYTE dt;
	ShortXLUnicodeString stName;
};

} // namespace XLS

