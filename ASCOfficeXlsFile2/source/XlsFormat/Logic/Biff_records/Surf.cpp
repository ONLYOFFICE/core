#include "precompiled_xls.h"
#include "Surf.h"

namespace XLS
{;

Surf::Surf()
{
}


Surf::~Surf()
{
}


BaseObjectPtr Surf::clone()
{
	return BaseObjectPtr(new Surf(*this));
}


void Surf::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fFillSurface);
	SETBIT(flags, 1, f3DPhongShade);
	record << flags;
}


void Surf::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fFillSurface = GETBIT(flags, 0);
	f3DPhongShade = GETBIT(flags, 1);
}

} // namespace XLS

