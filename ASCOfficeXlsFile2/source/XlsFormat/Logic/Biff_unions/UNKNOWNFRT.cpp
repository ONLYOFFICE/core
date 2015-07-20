//
#include "UNKNOWNFRT.h"
#include <Logic/Biff_records/SXAddl.h>

namespace XLS
{;


UNKNOWNFRT::UNKNOWNFRT()
{
}


UNKNOWNFRT::~UNKNOWNFRT()
{
}


BaseObjectPtr UNKNOWNFRT::clone()
{
	return BaseObjectPtr(new UNKNOWNFRT(*this));
}


// UNKNOWNFRT = SXAddl
const bool UNKNOWNFRT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl>())
	{
		return false;
	}
	return true;
}

} // namespace XLS

