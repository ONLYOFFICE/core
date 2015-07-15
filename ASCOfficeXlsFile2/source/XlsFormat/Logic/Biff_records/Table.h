#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CellRangeRef.h>

namespace XLS
{;


// Logical representation of Table record in BIFF8
class Table: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Table)
	BASE_OBJECT_DEFINE_CLASS_NAME(Table)
public:
	Table();
	~Table();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeTable;

//-----------------------------
	Ref ref_;

	BIFF_BOOL fAlwaysCalc;
	BIFF_BOOL fRw;
	BIFF_BOOL fTbl2;
	BIFF_BOOL fDeleted1;
	BIFF_BOOL fDeleted2;

	BIFF_BSTR r1;
	BIFF_BSTR r2;

		//if(fTbl2)
		//{
		//	//BO_ATTRIB_MARKUP_ATTRIB(r2)
		//}
};

} // namespace XLS

