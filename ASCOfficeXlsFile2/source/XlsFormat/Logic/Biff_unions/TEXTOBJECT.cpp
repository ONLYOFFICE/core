#include "precompiled_xls.h"
#include "TEXTOBJECT.h"
#include <Logic/Biff_records/TxO.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{;


TEXTOBJECT::TEXTOBJECT()
{
}


TEXTOBJECT::~TEXTOBJECT()
{
}


BaseObjectPtr TEXTOBJECT::clone()
{
	return BaseObjectPtr(new TEXTOBJECT(*this));
}


// TEXTOBJECT = TxO *Continue
const bool TEXTOBJECT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<TxO>())
	{
		return false;
	}
	proc.repeated<Continue>(0, 0);

	return true;
}

} // namespace XLS

