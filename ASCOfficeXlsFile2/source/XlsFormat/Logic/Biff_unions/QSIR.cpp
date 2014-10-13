#include "precompiled_xls.h"
#include "QSIR.h"
#include <Logic/Biff_records/Qsir.h>
#include <Logic/Biff_records/Qsif.h>

namespace XLS
{;


QSIR::QSIR()
{
}


QSIR::~QSIR()
{
}


BaseObjectPtr QSIR::clone()
{
	return BaseObjectPtr(new QSIR(*this));
}


// QSIR = Qsir *Qsif
const bool QSIR::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Qsir>())
	{
		return false;
	}
	proc.repeated<Qsif>(0, 0);

	return true;
}

} // namespace XLS

