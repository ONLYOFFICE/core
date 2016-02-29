#pragma once

#include "BiffRecord.h"
#include "Logic/Biff_structures/CellRangeRef.h"

namespace XLS
{


// Logical representation of LRng record in BIFF8
class LRng: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(LRng)
	BASE_OBJECT_DEFINE_CLASS_NAME(LRng)
public:
	LRng();
	~LRng();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType type = typeLRng;

//-----------------------------
	_UINT16 crefRw;
	BiffStructurePtrVector refRow;
	_UINT16 crefCol;
	BiffStructurePtrVector refCol;
};

} // namespace XLS

