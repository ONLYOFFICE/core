#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of LeftMargin record in BIFF8
class LeftMargin: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(LeftMargin)
	BASE_OBJECT_DEFINE_CLASS_NAME(LeftMargin)
public:
	LeftMargin();
	~LeftMargin();

	BaseObjectPtr clone();

	static const ElementType	type = typeLeftMargin;

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	BIFF_DOUBLE num;

};

} // namespace XLS

