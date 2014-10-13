#include "precompiled_xls.h"
#include "DataLabExtContents.h"
#include <Logic/Biff_structures/FrtHeader.h>

namespace XLS
{;

DataLabExtContents::DataLabExtContents()
{
}


DataLabExtContents::~DataLabExtContents()
{
}


BaseObjectPtr DataLabExtContents::clone()
{
	return BaseObjectPtr(new DataLabExtContents(*this));
}


void DataLabExtContents::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_DataLabExtContents);
	record << frtHeader;

	unsigned __int16 flags = 0;
	SETBIT(flags, 0, fSerName);
	SETBIT(flags, 1, fCatName);
	SETBIT(flags, 2, fValue);
	SETBIT(flags, 3, fPercent);
	SETBIT(flags, 4, fBubSizes);

	record << flags << rgchSep;
}


void DataLabExtContents::readFields(CFRecord& record)
{
	FrtHeader frtHeader;
	record >> frtHeader;

	unsigned __int16 flags;
	record >> flags >> rgchSep;

	fSerName = GETBIT(flags, 0);
	fCatName = GETBIT(flags, 1);
	fValue = GETBIT(flags, 2);
	fPercent = GETBIT(flags, 3);
	fBubSizes = GETBIT(flags, 4);
}

} // namespace XLS

