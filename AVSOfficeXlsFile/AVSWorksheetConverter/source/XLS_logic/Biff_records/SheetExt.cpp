#include "stdafx.h"
#include "SheetExt.h"
#include <XLS_logic/Biff_structures/FrtHeader.h>

namespace XLS
{;

SheetExt::SheetExt()
{
}


SheetExt::~SheetExt()
{
}


BaseObjectPtr SheetExt::clone()
{
	return BaseObjectPtr(new SheetExt(*this));
}


void SheetExt::writeFields(CFRecord& record)
{
// 	FrtHeader frtHeader(rt_SheetExt);
// 	record << frtHeader;

#pragma message("####################### SheetExt record is not implemented")
	Log::error("SheetExt record is not implemented.");
	//record << some_value;
}


void SheetExt::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_SheetExt);
	record >> frtHeader;

	record >> cb;
	DWORD flags;
	record >> flags;
	icvPlain = static_cast<BYTE>(GETBITS(flags, 0, 6));

	if(0x00000028 == cb)
	{
		record >> sheetExtOptional;
	}
}

} // namespace XLS

