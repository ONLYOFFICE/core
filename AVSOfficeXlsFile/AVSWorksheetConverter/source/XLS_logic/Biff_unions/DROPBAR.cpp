#include "stdafx.h"
#include "DROPBAR.h"
#include <XLS_logic/Biff_records/DropBar.h>
#include <XLS_logic/Biff_records/Begin.h>
#include <XLS_logic/Biff_records/LineFormat.h>
#include <XLS_logic/Biff_records/AreaFormat.h>
#include <XLS_logic/Biff_unions/GELFRAME.h>
#include <XLS_logic/Biff_unions/SHAPEPROPS.h>
#include <XLS_logic/Biff_records/End.h>

namespace XLS
{;


DROPBAR::DROPBAR()
{
}


DROPBAR::~DROPBAR()
{
}


BaseObjectPtr DROPBAR::clone()
{
	return BaseObjectPtr(new DROPBAR(*this));
}


// DROPBAR = DropBar Begin LineFormat AreaFormat [GELFRAME] [SHAPEPROPS] End
const bool DROPBAR::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<DropBar>())
	{
		return false;
	}
	proc.mandatory<Begin>();
	proc.mandatory<LineFormat>();
	proc.mandatory<AreaFormat>();
	proc.optional<GELFRAME>();
	proc.optional<SHAPEPROPS>();
	proc.mandatory<End>();

	return true;
}

} // namespace XLS

