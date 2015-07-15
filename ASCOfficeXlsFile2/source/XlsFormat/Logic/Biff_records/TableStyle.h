#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;


// Logical representation of TableStyle record in BIFF8
class TableStyle: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(TableStyle)
	BASE_OBJECT_DEFINE_CLASS_NAME(TableStyle)
public:
	TableStyle();
	~TableStyle();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeTableStyle;

//-----------------------------
	BIFF_BOOL fIsPivot;
	BIFF_BOOL fIsTable;
	BIFF_DWORD ctse;
	LPWideStringNoCch rgchName;


};

} // namespace XLS

