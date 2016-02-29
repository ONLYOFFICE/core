#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/ISSTInf.h>

namespace XLS
{


// Logical representation of ExtSST record in BIFF8
class ExtSST: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(ExtSST)
	BASE_OBJECT_DEFINE_CLASS_NAME(ExtSST)
public:
	ExtSST();
	~ExtSST();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeExtSST;

//-----------------------------
	_UINT16 dsst;
	BiffStructurePtrVector rgISSTInf;
	BackwardOnlyParam<_UINT32> num_sets;

};

} // namespace XLS

