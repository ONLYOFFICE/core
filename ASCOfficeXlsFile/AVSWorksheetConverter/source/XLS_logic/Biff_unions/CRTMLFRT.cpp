#include "stdafx.h"
#include "CRTMLFRT.h"
#include <XLS_logic/Biff_records/CrtMlFrt.h>
#include <XLS_logic/Biff_records/CrtMlFrtContinue.h>

namespace XLS
{;


CRTMLFRT::CRTMLFRT()
{
}


CRTMLFRT::~CRTMLFRT()
{
}


BaseObjectPtr CRTMLFRT::clone()
{
	return BaseObjectPtr(new CRTMLFRT(*this));
}


// CRTMLFRT = CrtMlFrt *CrtMlFrtContinue
const bool CRTMLFRT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<CrtMlFrt>())
	{
		return false;
	}
	proc.repeated<CrtMlFrtContinue>(0, 0);

	return true;
}

} // namespace XLS

