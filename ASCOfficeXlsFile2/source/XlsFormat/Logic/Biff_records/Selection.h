#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CellRangeRef.h>
#include <Logic/Biff_structures/PaneType.h>

namespace XLS
{;


// Logical representation of Selection record in BIFF8
class Selection: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Selection)
	BASE_OBJECT_DEFINE_CLASS_NAME(Selection)
public:
	Selection();
	~Selection();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	PaneType pnn;
	R_RwU rwAct;
	ColU colAct;
	BIFF_SHORT irefAct;
	BIFF_BSTR sqref;
	BIFF_BSTR activeCell;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(pnn)
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(rwAct.rw,L"rwAct")
// 		BO_ATTRIB_MARKUP_ATTRIB_NAME(colAct.col,L"colAct")
		BO_ATTRIB_MARKUP_ATTRIB(activeCell)
		BO_ATTRIB_MARKUP_ATTRIB(irefAct)
		BO_ATTRIB_MARKUP_ATTRIB(sqref)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

