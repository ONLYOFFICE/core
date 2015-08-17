
#include "FrtHeader.h"
#include "../../Binary/CFRecord.h"

namespace XLS
{;

FrtHeader::FrtHeader()
{
}


FrtHeader::FrtHeader(const CFRecordType::TypeId type)
:	rt(type)
{
}


BiffStructurePtr FrtHeader::clone()
{
	return BiffStructurePtr(new FrtHeader(*this));
}

void FrtHeader::store(CFRecord& record)
{
	record << rt << grbitFrt;
	record.reserveNunBytes(8); // reserved
}


void FrtHeader::load(CFRecord& record)
{
	record >> rt >> grbitFrt;
	record.skipNunBytes(8); // reserved
}


} // namespace XLS

