#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of BottomMargin record in BIFF8
class BottomMargin: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(BottomMargin)
	BASE_OBJECT_DEFINE_CLASS_NAME(BottomMargin)
public:
	BottomMargin();
	~BottomMargin();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeBottomMargin;

//-----------------------------
	BIFF_DOUBLE num;

};

} // namespace XLS

