
#include "SUB.h"
//#include <Logic/Biff_records/InterfaceHdr.h>
//#include <Logic/Biff_unions/InterfaceHdr.h>

namespace XLS
{


SUB::SUB()
{
}


SUB::~SUB()
{
}


BaseObjectPtr SUB::clone()
{
	return BaseObjectPtr(new SUB(*this));
}


// SUB = ????????????????????????????????????????
const bool SUB::loadContent(BinProcessor& proc)
{
#pragma message("####################### SUB union is not implemented")
	//Log::info("SUB union is not implemented!!!!!!!!!!!!!!!");

/*
	if(!proc.mandatory<InterfaceHdr>())
	{
		return false;
	}
	proc.mandatory<Mms>();
	proc.optional<Mms>();
	proc.repeated<Continue>(0, 0);
*/
	return false;
}

} // namespace XLS

