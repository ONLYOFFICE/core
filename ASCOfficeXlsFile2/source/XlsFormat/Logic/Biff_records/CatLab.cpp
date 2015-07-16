#include "precompiled_xls.h"
#include "CatLab.h"
#include <Logic/Biff_structures/FrtHeaderOld.h>

namespace XLS
{;

CatLab::CatLab()
{
}


CatLab::~CatLab()
{
}


BaseObjectPtr CatLab::clone()
{
	return BaseObjectPtr(new CatLab(*this));
}


void CatLab::writeFields(CFRecord& record)
{
	FrtHeaderOld frtHeaderOld(rt_CatLab);
	unsigned short flags = 0;
	SETBIT(flags, 0, cAutoCatLabelReal);
	record << frtHeaderOld << wOffset << at << flags;
	record.reserveNunBytes(2); // reserved
}


void CatLab::readFields(CFRecord& record)
{
	FrtHeaderOld frtHeaderOld;
	unsigned short flags;
	record >> frtHeaderOld >> wOffset >> at >> flags;
	cAutoCatLabelReal = GETBIT(flags, 0);
	record.skipNunBytes(2); // reserved
}

} // namespace XLS

