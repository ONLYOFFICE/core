#pragma once

#include "BiffRecordContinued.h"

namespace XLS
{;


// Logical representation of Pls record in BIFF8
class Pls: public BiffRecordContinued
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Pls)
	BASE_OBJECT_DEFINE_CLASS_NAME(Pls)
public:
	Pls(BaseObject* parent);
	~Pls();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePls;

//-----------------------------
	DEVMODE rgb;
	BIFF_LONG bin_data_id;
	BaseObject* parent_;


};

} // namespace XLS

