#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{;


// Logical representation of BigName record in BIFF8
class BigName: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BigName)
	BASE_OBJECT_DEFINE_CLASS_NAME(BigName)
public:
	BigName();
	~BigName();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeBigName;

//-----------------------------
//	BIFF_WORD userName;

};

} // namespace XLS

