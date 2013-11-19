#include "stdafx.h"
#include "DropDownObjIds.h"
#include <XLS_logic/Biff_structures/FrtHeader.h>

namespace XLS
{;
//const wchar_t FileOffsetStr[] = L"FileOffset";
DropDownObjIds::DropDownObjIds()
{
}


DropDownObjIds::~DropDownObjIds()
{
}


BaseObjectPtr DropDownObjIds::clone()
{
	return BaseObjectPtr(new DropDownObjIds(*this));
}


void DropDownObjIds::writeFields(CFRecord& record)
{
#pragma message("####################### DropDownObjIds record is not implemented")
	Log::error("DropDownObjIds record is not implemented.");
	//record << some_value;
}


void DropDownObjIds::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_DropDownObjIds);
	record >> frtHeader;
	WORD cidObj;
	record >> cidObj;
	for(WORD i = 0; i < cidObj; ++i)
	{
		DropDownObjIdPtr id(new DropDownObjId);
		record >> *id;
		rgidObj.push_back(id);
	}
}

} // namespace XLS

