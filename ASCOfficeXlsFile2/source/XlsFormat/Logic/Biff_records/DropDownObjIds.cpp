
#include "DropDownObjIds.h"
#include <Logic/Biff_structures/FrtHeader.h>

namespace XLS
{
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

}


void DropDownObjIds::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_DropDownObjIds);
	record >> frtHeader;
	unsigned short cidObj;
	record >> cidObj;
	for(unsigned short i = 0; i < cidObj; ++i)
	{
		DropDownObjIdPtr id(new DropDownObjId);
		record >> *id;
		rgidObj.push_back(id);
	}
}

} // namespace XLS

