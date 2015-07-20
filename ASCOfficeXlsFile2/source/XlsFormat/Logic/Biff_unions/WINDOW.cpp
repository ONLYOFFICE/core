
#include "WINDOW.h"
#include <Logic/Biff_records/Window2.h>
#include <Logic/Biff_records/PLV.h>
#include <Logic/Biff_records/Scl.h>
#include <Logic/Biff_records/Pane.h>
#include <Logic/Biff_records/Selection.h>

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

