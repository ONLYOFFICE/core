#include "stdafx.h"
#include "DAT.h"
#include <XLS_logic/Biff_records/Dat.h>
#include <XLS_logic/Biff_records/Begin.h>
#include <XLS_logic/Biff_unions/LD.h>
#include <XLS_logic/Biff_records/End.h>

namespace XLS
{;


DAT::DAT()
{
}


DAT::~DAT()
{
}


BaseObjectPtr DAT::clone()
{
	return BaseObjectPtr(new DAT(*this));
}


// DAT = Dat Begin LD End
const bool DAT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Dat>())
	{
		return false;
	}
	proc.mandatory<Begin>();
	proc.mandatory<LD>();
	proc.mandatory<End>();

	return true;
}

} // namespace XLS

