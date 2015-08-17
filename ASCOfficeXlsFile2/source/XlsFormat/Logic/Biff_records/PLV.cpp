
#include "PLV.h"
#include <Logic/Biff_structures/FrtHeader.h>

namespace XLS
{

PLV::PLV()
{
}


PLV::~PLV()
{
}


BaseObjectPtr PLV::clone()
{
	return BaseObjectPtr(new PLV(*this));
}


void PLV::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_PLV);
	record << frtHeader;
	unsigned short flags = 0;
	SETBIT(flags, 0, fPageLayoutView);
	SETBIT(flags, 1, fRulerVisible);
	SETBIT(flags, 2, fWhitespaceHidden);
	record << wScalePLV << flags;
}


void PLV::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_PLV);
	record >> frtHeader;
	unsigned short flags;
	record >> wScalePLV >> flags;
	fPageLayoutView = GETBIT(flags, 0);
	fRulerVisible = GETBIT(flags, 1);
	fWhitespaceHidden = GETBIT(flags, 2);
}

} // namespace XLS

