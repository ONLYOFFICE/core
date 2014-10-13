#include "precompiled_xls.h"
#include "LineFormat.h"

namespace XLS
{;

LineFormat::LineFormat()
{
}


LineFormat::~LineFormat()
{
}


BaseObjectPtr LineFormat::clone()
{
	return BaseObjectPtr(new LineFormat(*this));
}


void LineFormat::writeFields(CFRecord& record)
{
	unsigned __int16 flags = 0;
	SETBIT(flags, 0, fAuto);
	SETBIT(flags, 2, fAxisOn);
	SETBIT(flags, 3, fAutoCo);
	record << rgb << lns << we << flags << icv;
}


void LineFormat::readFields(CFRecord& record)
{
	unsigned __int16 flags;
	record >> rgb >> lns >> we >> flags >> icv;
	fAuto = GETBIT(flags, 0);
	fAxisOn = GETBIT(flags, 2);
	fAutoCo = GETBIT(flags, 3);
}

} // namespace XLS

