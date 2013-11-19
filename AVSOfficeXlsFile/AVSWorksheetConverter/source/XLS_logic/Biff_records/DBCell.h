#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/FileOffset.h>

namespace XLS
{;


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

	static void catchUnusedCellOffset(CFStream& stream, const DWORD data_place, const DWORD data);
	static void offsetFromRowMaker(CFStream& stream, const DWORD data_place, const DWORD data);

private:
	ForwardOnlyParam<DWORD> dbRtrw;
	BiffStructurePtrVector rgdb;
	BackwardOnlyParam<DWORD> num_pointers;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(dbRtrw)
		BO_ATTRIB_MARKUP_ATTRIB(num_pointers)
		BO_ATTRIB_MARKUP_VECTOR_COMPLEX(rgdb, FileOffset)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

