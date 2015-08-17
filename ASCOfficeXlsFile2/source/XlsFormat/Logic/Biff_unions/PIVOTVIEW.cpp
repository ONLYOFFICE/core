
#include "PIVOTVIEW.h"
#include <Logic/Biff_unions/PIVOTCORE.h>
#include <Logic/Biff_unions/PIVOTFRT.h>

namespace XLS
{


PIVOTVIEW::PIVOTVIEW()
{
}


PIVOTVIEW::~PIVOTVIEW()
{
}


BaseObjectPtr PIVOTVIEW::clone()
{
	return BaseObjectPtr(new PIVOTVIEW(*this));
}


// PIVOTVIEW = PIVOTCORE [PIVOTFRT]
const bool PIVOTVIEW::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<PIVOTCORE>())
	{
		return false;
	}
	proc.optional<PIVOTFRT>();

	return true;
}

} // namespace XLS

