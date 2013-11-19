#include "stdafx.h"
#include "THEME.h"
#include <XLS_logic/Biff_records/Theme.h>
#include <XLS_logic/Biff_records/ContinueFrt12.h>

namespace XLS
{;


THEME::THEME()
{
}


THEME::~THEME()
{
}


BaseObjectPtr THEME::clone()
{
	return BaseObjectPtr(new THEME(*this));
}


// THEME = Theme *ContinueFrt12
const bool THEME::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Theme>())
	{
		return false;
	}
	proc.repeated<ContinueFrt12>(0, 0);
	return true;
}

} // namespace XLS

