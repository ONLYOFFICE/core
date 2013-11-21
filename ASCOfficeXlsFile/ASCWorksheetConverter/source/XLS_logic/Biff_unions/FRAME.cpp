#include "stdafx.h"
#include "FRAME.h"
#include <XLS_logic/Biff_records/Frame.h>
#include <XLS_logic/Biff_records/Begin.h>
#include <XLS_logic/Biff_records/LineFormat.h>
#include <XLS_logic/Biff_records/AreaFormat.h>
#include <XLS_logic/Biff_records/End.h>

#include <XLS_logic/Biff_unions/GELFRAME.h>
#include <XLS_logic/Biff_unions/SHAPEPROPS.h>

namespace XLS
{;


FRAME::FRAME()
{
}


FRAME::~FRAME()
{
}


BaseObjectPtr FRAME::clone()
{
	return BaseObjectPtr(new FRAME(*this));
}


// FRAME = Frame Begin LineFormat AreaFormat [GELFRAME] [SHAPEPROPS] End
const bool FRAME::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Frame>())
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

