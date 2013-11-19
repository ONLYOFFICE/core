#include "stdafx.h"
#include "CrtLayout12.h"
#include <XLS_logic/Biff_structures/FrtHeader.h>

namespace XLS
{;

CrtLayout12::CrtLayout12()
{
}


CrtLayout12::~CrtLayout12()
{
}


BaseObjectPtr CrtLayout12::clone()
{
	return BaseObjectPtr(new CrtLayout12(*this));
}


void CrtLayout12::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_CrtLayout12);
	record << frtHeader;

	WORD flags = 0;
	SETBITS(flags, 1, 4, autolayouttype);
	record << dwCheckSum << flags << wXMode << wYMode << wWidthMode << wHeightMode << x << y << dx << dy;
	record.reserveNBytes(2); // reserved2
}


void CrtLayout12::readFields(CFRecord& record)
{
	FrtHeader frtHeader;
	record >> frtHeader;

	WORD flags;
	record >> dwCheckSum >> flags >> wXMode >> wYMode >> wWidthMode >> wHeightMode >> x >> y >> dx >> dy;
	autolayouttype = GETBITS(flags, 1, 4);
	record.skipNBytes(2); // reserved2
}

} // namespace XLS

