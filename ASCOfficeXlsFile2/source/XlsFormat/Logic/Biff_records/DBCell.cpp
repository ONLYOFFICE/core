#include "precompiled_xls.h"
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

void DBCell::catchUnusedCellOffset(CFStream& stream, const unsigned int data_place, const unsigned int data)
{
	// do nothing, just catch to remove it from SourceItems
}


void DBCell::offsetFromRowMaker(CFStream& stream, const unsigned int data_place, const unsigned int data)
{
	stream.seekFromBegin(data_place);
	unsigned int offset_from_Row = data_place - sizeof(unsigned __int16)/*size_short*/ - sizeof(CFRecordType::TypeId) - data /* Row file ptr */;
	stream.write(&offset_from_Row, sizeof(unsigned int)/*dbRtrw*/);
}


void DBCell::writeFields(CFRecord& record)
{
	record.registerDelayedFilePointerSource(rt_Index);
	record.registerDelayedDataReceiver(&offsetFromRowMaker, sizeof(unsigned int) /*dbRtrw*/); // From the first Row in the current block
	record.registerDelayedDataReceiver(&catchUnusedCellOffset, 0, rt_Blank);

	for(unsigned int i = 0; i < num_pointers ; ++i)
	{
		record.registerDelayedDataReceiver(NULL, sizeof(unsigned __int16)/*FilePointer*/);
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

