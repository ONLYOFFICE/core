#pragma once

#include "BiffRecord.h"

namespace XLS
{


// Logical representation of TopMargin record in BIFF8
class TopMargin: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(TopMargin)
	BASE_OBJECT_DEFINE_CLASS_NAME(TopMargin)
public:
	TopMargin();
	~TopMargin();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeTopMargin;

//-----------------------------
	BIFF_DOUBLE num;

};

} // namespace XLS

