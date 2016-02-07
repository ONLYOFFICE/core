#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CellRangeRef.h>
#include <Logic/Biff_structures/PaneType.h>

namespace XLS
{


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

	static const ElementType	type = typeSelection;

	int serialize(std::wostream & stream);

//-----------------------------
	PaneType	pnn;

	R_RwU		rwAct;
	ColU		colAct;
	BIFF_SHORT	irefAct;
	
	BIFF_BSTR	sqref;
	BIFF_BSTR	activeCell;
};

} // namespace XLS

