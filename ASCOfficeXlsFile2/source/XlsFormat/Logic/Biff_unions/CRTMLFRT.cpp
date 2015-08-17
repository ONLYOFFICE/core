
#include "CRTMLFRT.h"
#include <Logic/Biff_records/CrtMlFrt.h>
#include <Logic/Biff_records/CrtMlFrtContinue.h>

namespace XLS
{


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

