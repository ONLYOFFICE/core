#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of WOpt record in BIFF8
class WOpt: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(WOpt)
	BASE_OBJECT_DEFINE_CLASS_NAME(WOpt)
public:
	WOpt();
	~WOpt();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeWOpt;

//-----------------------------
//	BIFF_WORD userName;

};

} // namespace XLS

