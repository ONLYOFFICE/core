#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/FileOffset.h>

namespace XLS
{


// Logical representation of DBCell record in BIFF8
class DBCell: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(DBCell)
	BASE_OBJECT_DEFINE_CLASS_NAME(DBCell)
public:
	DBCell();
	~DBCell();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static void catchUnusedCellOffset(CFStream& stream, const unsigned int data_place, const unsigned int data);
	static void offsetFromRowMaker(CFStream& stream, const unsigned int data_place, const unsigned int data);

	static const ElementType	type = typeDBCell;
//-----------------------------
	ForwardOnlyParam<_UINT32> dbRtrw;
	BiffStructurePtrVector rgdb;
	BackwardOnlyParam<_UINT32> num_pointers;
};

} // namespace XLS

