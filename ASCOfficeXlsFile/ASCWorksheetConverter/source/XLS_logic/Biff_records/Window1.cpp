#include "stdafx.h"
#include "Window1.h"

namespace XLS
{;

Window1::Window1()
{
}


Window1::~Window1()
{
}


BaseObjectPtr Window1::clone()
{
	return BaseObjectPtr(new Window1(*this));
}


void Window1::writeFields(CFRecord& record)
{
	record << xWn << yWn << dxWn << dyWn;
	WORD flags = 0;
	SETBIT(flags, 0, fHidden);
	SETBIT(flags, 1, fIconic);
	SETBIT(flags, 2, fVeryHidden);
	SETBIT(flags, 3, fDspHScroll);
	SETBIT(flags, 4, fDspVScroll);
	SETBIT(flags, 5, fBotAdornment);
	SETBIT(flags, 6, fNoAFDateGroup);
	record << flags << itabCur << itabFirst << ctabSel << wTabRatio;
}


void Window1::readFields(CFRecord& record)
{
	WORD flags;
	record >> xWn >> yWn >> dxWn >> dyWn >> flags >> itabCur >> itabFirst >> ctabSel >> wTabRatio;
	fHidden = GETBIT(flags, 0);
	fIconic = GETBIT(flags, 1);
	fVeryHidden = GETBIT(flags, 2);
	fDspHScroll = GETBIT(flags, 3);
	fDspVScroll = GETBIT(flags, 4);
	fBotAdornment = GETBIT(flags, 5);
	fNoAFDateGroup = GETBIT(flags, 6);
}

} // namespace XLS

