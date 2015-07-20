
#include "DROPBAR.h"
#include <Logic/Biff_records/DropBar.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/LineFormat.h>
#include <Logic/Biff_records/AreaFormat.h>
#include <Logic/Biff_unions/GELFRAME.h>
#include <Logic/Biff_unions/SHAPEPROPS.h>
#include <Logic/Biff_records/End.h>

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

