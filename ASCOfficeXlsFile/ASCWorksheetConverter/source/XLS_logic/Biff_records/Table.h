#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/CellRangeRef.h>

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
private:
	Ref ref_;

	BIFF_BOOL fAlwaysCalc;
	BIFF_BOOL fRw;
	BIFF_BOOL fTbl2;
	BIFF_BOOL fDeleted1;
	BIFF_BOOL fDeleted2;

	BIFF_BSTR r1;
	BIFF_BSTR r2;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(ref_)
		BO_ATTRIB_MARKUP_ATTRIB(fAlwaysCalc)
		BO_ATTRIB_MARKUP_ATTRIB(fRw)
		BO_ATTRIB_MARKUP_ATTRIB(fTbl2)
		BO_ATTRIB_MARKUP_ATTRIB(fDeleted1)
		BO_ATTRIB_MARKUP_ATTRIB(fDeleted2)
		BO_ATTRIB_MARKUP_ATTRIB(r1)
		if(fTbl2)
		{
			BO_ATTRIB_MARKUP_ATTRIB(r2)
		}
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

