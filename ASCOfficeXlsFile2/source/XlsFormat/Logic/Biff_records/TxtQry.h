#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of TxtQry record in BIFF8
class TxtQry: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(TxtQry)
	BASE_OBJECT_DEFINE_CLASS_NAME(TxtQry)
public:
	TxtQry();
	~TxtQry();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeTxtQry;

//-----------------------------
//	BIFF_WORD userName;

};

} // namespace XLS

