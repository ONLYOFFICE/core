
#include "CrtLayout12.h"
#include <Logic/Biff_structures/FrtHeader.h>

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

	unsigned short flags = 0;
	SETBITS(flags, 1, 4, autolayouttype);
	record << dwCheckSum << flags << wXMode << wYMode << wWidthMode << wHeightMode << x << y << dx << dy;
	record.reserveNunBytes(2); // reserved2
}


void CrtLayout12::readFields(CFRecord& record)
{
	FrtHeader frtHeader;
	record >> frtHeader;

	unsigned short flags;
	record >> dwCheckSum >> flags >> wXMode >> wYMode >> wWidthMode >> wHeightMode >> x >> y >> dx >> dy;
	autolayouttype = GETBITS(flags, 1, 4);
	record.skipNunBytes(2); // reserved2
}

} // namespace XLS

