#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Guts record in BIFF8
class Guts: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Guts)
	BASE_OBJECT_DEFINE_CLASS_NAME(Guts)
public:
	Guts();
	~Guts();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeGuts;

//-----------------------------
	BIFF_WORD iLevelRwMac;
	BIFF_WORD iLevelColMac;

};

} // namespace XLS

