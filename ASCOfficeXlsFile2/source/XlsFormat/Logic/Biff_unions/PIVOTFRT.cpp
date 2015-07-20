
#include "PIVOTFRT.h"
#include <Logic/Biff_unions/PIVOTFRT9.h>
#include <Logic/Biff_unions/PIVOTADDL.h>

namespace XLS
{;


PIVOTFRT::PIVOTFRT()
{
}


PIVOTFRT::~PIVOTFRT()
{
}


BaseObjectPtr PIVOTFRT::clone()
{
	return BaseObjectPtr(new PIVOTFRT(*this));
}


// PIVOTFRT = PIVOTFRT9 [PIVOTADDL]
const bool PIVOTFRT::loadContent(BinProcessor& proc)
{

	if(!proc.mandatory<PIVOTFRT9>())
	{
		return false;
	}
#pragma message("####################### PIVOTFRT union is not implemented")
	Log::info("PIVOTFRT union is not implemented!!!!!!!!!!!!!!!");
	proc.optional<PIVOTADDL>();
	return true;
}

} // namespace XLS

