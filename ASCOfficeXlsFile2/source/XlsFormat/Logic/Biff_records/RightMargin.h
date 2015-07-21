#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of RightMargin record in BIFF8
class RightMargin: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(RightMargin)
	BASE_OBJECT_DEFINE_CLASS_NAME(RightMargin)
public:
	RightMargin();
	~RightMargin();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeRightMargin;

//-----------------------------
	BIFF_DOUBLE num;

};

} // namespace XLS

