
#include "FrtRefHeader.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr FrtRefHeader::clone()
{
	return BiffStructurePtr(new FrtRefHeader(*this));
}



void FrtRefHeader::store(CFRecord& record)
{
	record << rt << grbitFrt;
	record << ref8;
}


void FrtRefHeader::load(CFRecord& record)
{
	record >> rt >> grbitFrt;
	record >> ref8;
}


} // namespace XLS

