#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CellRef.h>
#include <Logic/Biff_structures/PaneType.h>

namespace XLS
{


// Logical representation of Pane record in BIFF8
class Pane: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Pane)
	BASE_OBJECT_DEFINE_CLASS_NAME(Pane)
public:
	Pane();
	~Pane();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typePane;

//-----------------------------
	_UINT16		x;
	_UINT16		y;
	_UINT16		rwTop;
	_UINT16		colLeft;
	PaneType	pnnAcct;
	
	BIFF_BSTR	topLeftCell;
};

} // namespace XLS

