#include "stdafx.h"
#include "AXISPARENT.h"
#include <XLS_logic/Biff_records/AxisParent.h>
#include <XLS_logic/Biff_records/Begin.h>
#include <XLS_logic/Biff_records/Pos.h>
#include <XLS_logic/Biff_records/End.h>

#include <XLS_logic/Biff_unions/AXES.h>
#include <XLS_logic/Biff_unions/CRT.h>

namespace XLS
{;


AXISPARENT::AXISPARENT()
{
}


AXISPARENT::~AXISPARENT()
{
}


BaseObjectPtr AXISPARENT::clone()
{
	return BaseObjectPtr(new AXISPARENT(*this));
}


// AXISPARENT = AxisParent Begin Pos [AXES] 1*4CRT End
const bool AXISPARENT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<AxisParent>())
	{
		return false;
	}
	proc.mandatory<Begin>();
	//proc.mandatory<Pos>();

	// fix
	proc.optional<Pos>();

	proc.optional<AXES>();
	proc.repeated<CRT>(1, 4);
	proc.mandatory<End>();

	return true;
}

} // namespace XLS

