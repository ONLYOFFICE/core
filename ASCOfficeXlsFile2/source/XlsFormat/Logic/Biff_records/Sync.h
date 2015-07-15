#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of Sync record in BIFF8
class Sync: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Sync)
	BASE_OBJECT_DEFINE_CLASS_NAME(Sync)
public:
	Sync();
	~Sync();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeSync;

//-----------------------------
	ForwardOnlyParam<unsigned __int16> rw;
	ForwardOnlyParam<unsigned __int16> col;
	BIFF_BSTR ref_;

};

} // namespace XLS

