
#include "DAT.h"
#include <Logic/Biff_records/Dat.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_unions/LD.h>
#include <Logic/Biff_records/End.h>

namespace XLS
{


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

