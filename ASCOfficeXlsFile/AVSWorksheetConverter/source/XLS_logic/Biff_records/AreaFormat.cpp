#include "stdafx.h"
#include "AreaFormat.h"

namespace XLS
{;

AreaFormat::AreaFormat()
{
}


AreaFormat::~AreaFormat()
{
}


BaseObjectPtr AreaFormat::clone()
{
	return BaseObjectPtr(new AreaFormat(*this));
}


void AreaFormat::writeFields(CFRecord& record)
{
	WORD flags = 0;
	SETBIT(flags, 0, fAuto);
	SETBIT(flags, 1, fInvertNeg);
	record << rgbFore << rgbBack << fls << flags << icvFore << icvBack;
}


void AreaFormat::readFields(CFRecord& record)
{
	WORD flags;
	record >> rgbFore >> rgbBack >> fls >> flags >> icvFore >> icvBack;
	fAuto = GETBIT(flags, 0);
	fInvertNeg = GETBIT(flags, 1);
}

} // namespace XLS

