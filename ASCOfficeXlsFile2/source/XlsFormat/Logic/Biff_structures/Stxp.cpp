
#include "Stxp.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr Stxp::clone()
{
	return BiffStructurePtr(new Stxp(*this));
}

void Stxp::store(CFRecord& record)
{
	record << twpHeight << ts << bls << sss << uls << bFamily << bCharSet;
	record.reserveNunBytes(1); // unused
}


void Stxp::load(CFRecord& record)
{
	record >> twpHeight >> ts >> bls >> sss >> uls >> bFamily >> bCharSet;
	record.skipNunBytes(1); // unused
}


} // namespace XLS

