#pragma once

#include "BiffRecord.h"
#include "XLS_logic/Biff_structures/CellRangeRef.h"

namespace XLS
{;


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
private:
	BIFF_WORD crefRw;
	BiffStructurePtrVector refRow;
	BIFF_WORD crefCol;
	BiffStructurePtrVector refCol;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(crefRw)
		BO_ATTRIB_MARKUP_VECTOR_COMPLEX(refRow, Ref8U)
		BO_ATTRIB_MARKUP_ATTRIB(crefCol)
		BO_ATTRIB_MARKUP_VECTOR_COMPLEX(refCol, Ref8U)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

