#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of NameFnGrp12 record in BIFF8
class NameFnGrp12: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(NameFnGrp12)
	BASE_OBJECT_DEFINE_CLASS_NAME(NameFnGrp12)
public:
	NameFnGrp12();
	~NameFnGrp12();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	_UINT16 cachName;
	_UINT16 fgrp;

	XLUnicodeString rgach;

};

} // namespace XLS

