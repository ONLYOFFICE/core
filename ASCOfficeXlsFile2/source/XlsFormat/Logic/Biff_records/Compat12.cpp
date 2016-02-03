
#include "Compat12.h"
#include <Logic/Biff_structures/FrtHeader.h>

namespace XLS
{

Compat12::Compat12()
{
}


Compat12::~Compat12()
{
}


BaseObjectPtr Compat12::clone()
{
	return BaseObjectPtr(new Compat12(*this));
}


void Compat12::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_Compat12);
	record << frtHeader;
	record << fNoCompatChk;
}


void Compat12::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_Compat12);
	
	record >> frtHeader;

	_UINT32 flag = 0;
	record >> flag;
	
	fNoCompatChk = (bool)flag;
}

} // namespace XLS

