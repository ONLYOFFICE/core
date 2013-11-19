#include "stdafx.h"
#include "CatLab.h"
#include <XLS_logic/Biff_structures/FrtHeaderOld.h>

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
	WORD flags = 0;
	SETBIT(flags, 0, cAutoCatLabelReal);
	record << frtHeaderOld << wOffset << at << flags;
	record.reserveNBytes(2); // reserved
}


void CatLab::readFields(CFRecord& record)
{
	FrtHeaderOld frtHeaderOld;
	WORD flags;
	record >> frtHeaderOld >> wOffset >> at >> flags;
	cAutoCatLabelReal = GETBIT(flags, 0);
	record.skipNBytes(2); // reserved
}

} // namespace XLS

