#include "stdafx.h"
#include "XF.h"

namespace XLS
{;

XF::XF(size_t& cell_xf_current_id, size_t& style_xf_current_id)
:	cell(cell_xf_current_id, style_xf_current_id),
	style(cell_xf_current_id, style_xf_current_id)
{
}


XF::~XF()
{
}


BaseObjectPtr XF::clone()
{
	return BaseObjectPtr(new XF(*this));
}


void XF::writeFields(CFRecord& record)
{
	WORD flags = 0;
	SETBIT(flags, 0, fLocked);
	SETBIT(flags, 1, fHidden);
	SETBIT(flags, 2, fStyle);
	SETBIT(flags, 3, f123Prefix);
	SETBITS(flags, 4, 15, ixfParent);
	record << ifnt << ifmt << flags;
	if(fStyle)
	{
		style.store(record);
	}
	else
	{
		cell.store(record);
	}
}


void XF::readFields(CFRecord& record)
{
	WORD flags;
	record >> ifnt >> ifmt >> flags;
	fLocked = GETBIT(flags, 0);
	fHidden = GETBIT(flags, 1);
	fStyle = GETBIT(flags, 2);
	f123Prefix = GETBIT(flags, 3);
	ixfParent = GETBITS(flags, 4, 15);
	if(fStyle)
	{
		style.load(record);
	}
	else
	{
		cell.load(record);
	}
}


} // namespace XLS

