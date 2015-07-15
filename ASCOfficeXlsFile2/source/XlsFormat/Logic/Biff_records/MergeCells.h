#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/CellRangeRef.h>

namespace XLS
{;


// Logical representation of MergeCells record in BIFF8
class MergeCells: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MergeCells)
	BASE_OBJECT_DEFINE_CLASS_NAME(MergeCells)
public:
	MergeCells();
	~MergeCells();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeMergeCells;

//-----------------------------
	BIFF_WORD cmcs;
	BiffStructurePtrVector rgref;
public:
	//BO_ATTRIB_MARKUP_BEGIN
		//BO_ATTRIB_MARKUP_ATTRIB(cmcs)
		//BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgref, Ref8)
	//BO_ATTRIB_MARKUP_END

};

} // namespace XLS

