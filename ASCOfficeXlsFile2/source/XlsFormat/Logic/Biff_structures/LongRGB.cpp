
#include "LongRGB.h"
#include <Binary/CFRecord.h>

namespace XLS
{


LongRGBA::LongRGBA()
:	is_alpha(true)
{
}


LongRGBA::LongRGBA(const bool alpha_present)
:	is_alpha(alpha_present)
{
}


LongRGB::LongRGB()
:	LongRGBA(false)
{
}


BiffStructurePtr LongRGBA::clone()
{
	return BiffStructurePtr(new LongRGBA(*this));
}


BiffStructurePtr LongRGB::clone()
{
	return BiffStructurePtr(new LongRGB(*this));
}

void LongRGBA::store(CFRecord& record)
{
	record << red << green << blue;
	if(is_alpha)
	{
		record << alpha;
	}
	else
	{
		record.reserveNunBytes(1); // reserved
	}
}


void LongRGBA::load(CFRecord& record)
{
	record >> red >> green >> blue;
	if(is_alpha)
	{
		record >> alpha;
	}
	else
	{
		alpha = 0;
		record.skipNunBytes(1); // reserved
	}
	argb = STR::toARGB(red, green, blue, alpha).c_str();
}


} // namespace XLS
