#include "precompiled_xls.h"
#include "PIVOTVD.h"
#include <Logic/Biff_records/Sxvd.h>
#include <Logic/Biff_records/SXVI.h>
#include <Logic/Biff_records/SXVDEx.h>

namespace XLS
{;


PIVOTVD::PIVOTVD()
{
}


PIVOTVD::~PIVOTVD()
{
}


BaseObjectPtr PIVOTVD::clone()
{
	return BaseObjectPtr(new PIVOTVD(*this));
}


// PIVOTVD = Sxvd *SXVI SXVDEx
const bool PIVOTVD::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Sxvd>())
	{
		return false;
	}
	proc.repeated<SXVI>(0, 0);
	proc.mandatory<SXVDEx>();

	return true;
}

} // namespace XLS

