
#include "LbsDropData.h"
#include <Binary/CFRecord.h>
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{


BiffStructurePtr LbsDropData::clone()
{
	return BiffStructurePtr(new LbsDropData(*this));
}


void LbsDropData::store(CFRecord& record)
{
	unsigned short flags = 0;
	SETBITS(flags, 0, 1, wStyle);
	SETBIT(flags, 3, fFiltered);

	record << flags << cLine << dxMin << str;

	if(0 != (str.getStructSize() & 1))
	{
		record.reserveNunBytes(1); // padding
	}
}


void LbsDropData::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags >> cLine >> dxMin >> str;
	wStyle = GETBITS(flags, 0, 1);
	fFiltered = GETBIT(flags, 3);

	if(0 != (str.getStructSize() & 1))
	{
		record.skipNunBytes(1); // padding
	}

}


} // namespace XLS

