#include "stdafx.h"
#include "XFS.h"
#include <XLS_logic/Biff_records/XF.h>
#include <XLS_logic/Biff_records/XFCRC.h>
#include <XLS_logic/Biff_records/XFExt.h>

namespace XLS
{;


XFS::XFS()
:	cell_xf_current_id(0), style_xf_current_id(0)
{
}


XFS::~XFS()
{
}


BaseObjectPtr XFS::clone()
{
	return BaseObjectPtr(new XFS(*this));
}


// XFS = 16*XF [XFCRC 16*4050XFExt]
const bool XFS::loadContent(BinProcessor& proc)
{
	proc.repeated(XF(cell_xf_current_id, style_xf_current_id) ,16, 0);
	if(proc.optional<XFCRC>())
	{
		proc.repeated<XFExt>(16, 4050);
	}
	return true;
}

} // namespace XLS

