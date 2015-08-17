#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CellOffsetResender.h>
#include <Logic/Biff_structures/Cell.h>

namespace XLS
{


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

	int serialize(std::wostream & stream);

	const CellRef getLocation() const;

//-----------------------------
	CellOffsetResender resender;
	Cell cell;
	BIFF_DWORD isst;

};

} // namespace XLS

