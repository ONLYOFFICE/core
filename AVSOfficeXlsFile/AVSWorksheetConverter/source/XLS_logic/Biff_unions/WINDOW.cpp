#include "stdafx.h"
#include "WINDOW.h"
#include <XLS_logic/Biff_records/Window2.h>
#include <XLS_logic/Biff_records/PLV.h>
#include <XLS_logic/Biff_records/Scl.h>
#include <XLS_logic/Biff_records/Pane.h>
#include <XLS_logic/Biff_records/Selection.h>

namespace XLS
{;


WINDOW::WINDOW()
{
}


WINDOW::~WINDOW()
{
}


BaseObjectPtr WINDOW::clone()
{
	return BaseObjectPtr(new WINDOW(*this));
}


// WINDOW = Window2 [PLV] [Scl] [Pane] *Selection
const bool WINDOW::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Window2>())
	{
		return false;
	}
	proc.optional<PLV>();
	proc.optional<Scl>();
	proc.optional<Pane>();
	proc.repeated<Selection>(0, 0);

	return true;
}

} // namespace XLS

