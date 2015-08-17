
#include "PIVOTCORE.h"
#include <Logic/Biff_records/SxView.h>
#include <Logic/Biff_unions/PIVOTVD.h>
#include <Logic/Biff_unions/PIVOTIVD.h>
#include <Logic/Biff_unions/PIVOTPI.h>
#include <Logic/Biff_records/SXDI.h>
#include <Logic/Biff_unions/PIVOTLI.h>
#include <Logic/Biff_unions/PIVOTEX.h>

namespace XLS
{


PIVOTCORE::PIVOTCORE()
{
}


PIVOTCORE::~PIVOTCORE()
{
}


BaseObjectPtr PIVOTCORE::clone()
{
	return BaseObjectPtr(new PIVOTCORE(*this));
}


// PIVOTCORE = SxView *PIVOTVD *2PIVOTIVD [PIVOTPI] *SXDI *PIVOTLI PIVOTEX
const bool PIVOTCORE::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SxView>())
	{
		return false;
	}
	proc.repeated<PIVOTVD>(0, 0);
	proc.repeated<PIVOTIVD>(0, 2);
	proc.optional<PIVOTPI>();
	proc.repeated<SXDI>(0, 0);
	proc.repeated<PIVOTLI>(0, 0);
	proc.mandatory<PIVOTEX>();
	
	return true;
}

} // namespace XLS

