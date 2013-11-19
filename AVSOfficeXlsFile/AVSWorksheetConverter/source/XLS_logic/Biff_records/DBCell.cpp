#include "stdafx.h"
#include "DBCell.h"

namespace XLS
{;

DBCell::DBCell()
{
}


DBCell::~DBCell()
{
}


BaseObjectPtr DBCell::clone()
{
	return BaseObjectPtr(new DBCell(*this));
}

void DBCell::catchUnusedCellOffset(CFStream& stream, const DWORD data_place, const DWORD data)
{
	// do nothing, just catch to remove it from SourceItems
}


void DBCell::offsetFromRowMaker(CFStream& stream, const DWORD data_place, const DWORD data)
{
	stream.seekFromBegin(data_place);
	DWORD offset_from_Row = data_place - sizeof(WORD)/*size_short*/ - sizeof(CFRecordType::TypeId) - data /* Row file ptr */;
	stream.write(&offset_from_Row, sizeof(DWORD)/*dbRtrw*/);
}


void DBCell::writeFields(CFRecord& record)
{
	record.registerDelayedFilePointerSource(rt_Index);
	record.registerDelayedDataReceiver(&offsetFromRowMaker, sizeof(DWORD) /*dbRtrw*/); // From the first Row in the current block
	record.registerDelayedDataReceiver(&catchUnusedCellOffset, 0, rt_Blank);

	for(DWORD i = 0; i < num_pointers ; ++i)
	{
		record.registerDelayedDataReceiver(NULL, sizeof(WORD)/*FilePointer*/);
	}

}


void DBCell::readFields(CFRecord& record)
{
	record >> dbRtrw;
	while(!record.isEOF())
	{
		FileOffsetPtr element(new FileOffset);
		record >> *element;
		rgdb.push_back(element);
	}
}

} // namespace XLS

