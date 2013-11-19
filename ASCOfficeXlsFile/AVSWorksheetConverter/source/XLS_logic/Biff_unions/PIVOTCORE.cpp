#include "stdafx.h"
#include "PIVOTCORE.h"
#include <XLS_logic/Biff_records/SxView.h>
#include <XLS_logic/Biff_unions/PIVOTVD.h>
#include <XLS_logic/Biff_unions/PIVOTIVD.h>
#include <XLS_logic/Biff_unions/PIVOTPI.h>
#include <XLS_logic/Biff_records/SXDI.h>
#include <XLS_logic/Biff_unions/PIVOTLI.h>
#include <XLS_logic/Biff_unions/PIVOTEX.h>

namespace XLS
{;


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

