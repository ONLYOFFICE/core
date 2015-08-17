
#include "RECORD12.h"
#include <Logic/Biff_records/HeaderFooter.h>

namespace XLS
{


RECORD12::RECORD12()
{
}


RECORD12::~RECORD12()
{
}


BaseObjectPtr RECORD12::clone()
{
	return BaseObjectPtr(new RECORD12(*this));
}


// RECORD12 = HeaderFooter
const bool RECORD12::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<HeaderFooter>())
	{
		return false;
	}

	return true;
}

} // namespace XLS

