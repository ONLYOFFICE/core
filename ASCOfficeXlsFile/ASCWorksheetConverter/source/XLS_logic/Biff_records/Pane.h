#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/CellRef.h>
#include <XLS_logic/Biff_structures/PaneType.h>

namespace XLS
{;


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
private:
	BIFF_WORD x;
	BIFF_WORD y;
	R_RwU rwTop;
	ColU colLeft;
	PaneType pnnAcct;
	BIFF_BSTR topLeftCell;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(x)
		BO_ATTRIB_MARKUP_ATTRIB(y)
//		BO_ATTRIB_MARKUP_ATTRIB(rwTop)
//		BO_ATTRIB_MARKUP_ATTRIB(colLeft)
		BO_ATTRIB_MARKUP_ATTRIB(pnnAcct)
		BO_ATTRIB_MARKUP_ATTRIB(topLeftCell)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

