#include "stdafx.h"
#include "DataLabExtContents.h"
#include <XLS_logic/Biff_structures/FrtHeader.h>

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

	WORD flags = 0;
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

	WORD flags;
	record >> flags >> rgchSep;

	fSerName = GETBIT(flags, 0);
	fCatName = GETBIT(flags, 1);
	fValue = GETBIT(flags, 2);
	fPercent = GETBIT(flags, 3);
	fBubSizes = GETBIT(flags, 4);
}

} // namespace XLS

