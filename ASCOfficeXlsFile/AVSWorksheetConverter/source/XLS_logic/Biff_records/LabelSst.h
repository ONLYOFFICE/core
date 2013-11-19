#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/CellOffsetResender.h>
#include <XLS_logic/Biff_structures/Cell.h>

namespace XLS
{;


// Logical representation of LabelSst record in BIFF8
class LabelSst: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(LabelSst)
	BASE_OBJECT_DEFINE_CLASS_NAME(LabelSst)
public:
	LabelSst();
	~LabelSst();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	const CellRef getLocation() const;

private:
	CellOffsetResender resender;
	Cell cell;
	BIFF_DWORD isst;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(resender);
		BO_ATTRIB_MARKUP_COMPLEX(cell);
		BO_ATTRIB_MARKUP_ATTRIB(isst)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

