#include "precompiled_xls.h"
#include "Sort.h"

namespace XLS
{;

Sort::Sort()
{
}


Sort::~Sort()
{
}


BaseObjectPtr Sort::clone()
{
	return BaseObjectPtr(new Sort(*this));
}


void Sort::writeFields(CFRecord& record)
{
#pragma message("####################### Sort record is not implemented")
	Log::error("Sort record is not implemented.");
	//record << some_value;
}


void Sort::readFields(CFRecord& record)
{
	unsigned __int16 flags;
	record >> flags;
	fCol = GETBIT(flags, 0);
	fKey1Dsc = GETBIT(flags, 1);
	fKey2Dsc = GETBIT(flags, 2);
	fKey3Dsc = GETBIT(flags, 3);
	fCaseSensitive = GETBIT(flags, 4);
	iOrder = GETBITS(flags, 5, 9);
	fAltMethod = GETBIT(flags, 10);

	unsigned char cchKey1;
	unsigned char cchKey2;
	unsigned char cchKey3;
	record >> cchKey1 >> cchKey2 >> cchKey3;
	if(cchKey1)
	{
		stKey1.setSize(cchKey1);
		record >> stKey1;
	}
	if(cchKey2)
	{
		stKey2.setSize(cchKey2);
		record >> stKey2;
	}
	if(cchKey3)
	{
		stKey3.setSize(cchKey3);
		record >> stKey3;
	}
	record.skipNunBytes(1); // reserved
}

} // namespace XLS

