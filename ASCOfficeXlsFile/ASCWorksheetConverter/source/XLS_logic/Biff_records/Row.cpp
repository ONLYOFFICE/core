#include "stdafx.h"
#include "Row.h"

namespace XLS
{;

Row::Row()
{
}


Row::~Row()
{
}


BaseObjectPtr Row::clone()
{
	return BaseObjectPtr(new Row(*this));
}


void Row::writeFields(CFRecord& record)
{
	if(1 == in_block_position)
	{
		record.registerDelayedFilePointerSource(rt_DBCell); // For DBCell::dbRtrw 
	}

	record << rw << colMic << colMac << miyRw;
	record.reserveNBytes(4); // reserved / unused
	WORD flags = 0x0100;
	SETBITS(flags, 0, 2, iOutLevel);
	SETBIT(flags, 4, fCollapsed);
	SETBIT(flags, 5, fDyZero);
	SETBIT(flags, 6, fUnsynced);
	SETBIT(flags, 7, fGhostDirty);
	record << flags;

	flags = 0;
	SETBITS(flags, 0, 11, ixfe_val);
	SETBIT(flags, 12, fExAsc);
	SETBIT(flags, 13, fExDes);
	SETBIT(flags, 14, fPhonetic);
	record << flags;

	if(1 == in_block_position)
	{
		record.registerDelayedFilePointerAndOffsetSource(record.getDataSize() + sizeof(WORD)/*size_short*/ + sizeof(CFRecordType::TypeId), rt_Blank); // For CELL container. All Cell containers will listen for rt_Blank
	}
}


void Row::readFields(CFRecord& record)
{
	record >> rw >> colMic >> colMac >> miyRw;
	record.skipNBytes(4); // reserved / unused
	WORD flags;
	record >> flags;
	iOutLevel = GETBITS(flags, 0, 2);
	fCollapsed = GETBIT(flags, 4);
	fDyZero = GETBIT(flags, 5);
	fUnsynced = GETBIT(flags, 6);
	fGhostDirty = GETBIT(flags, 7);

	record >> flags;
	ixfe_val = GETBITS(flags, 0, 11);
	fExAsc = GETBIT(flags, 12);
	fExDes = GETBIT(flags, 13);
	fPhonetic = GETBIT(flags, 14);
}

} // namespace XLS

