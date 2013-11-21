#include "stdafx.h"
#include "ChartParsedFormula.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;

ChartParsedFormula::ChartParsedFormula()
:	ParsedFormula(CellRef())
{
}


BiffStructurePtr ChartParsedFormula::clone()
{
	return BiffStructurePtr(new ChartParsedFormula(*this));
}


void ChartParsedFormula::store(CFRecord& record)
{
	record.registerDelayedDataReceiver(NULL, sizeof(WORD)/*cce*/);
	rgce.store(record);
	record.registerDelayedDataSource(rgce.getCCE(), rt_Formula);
}


void ChartParsedFormula::load(CFRecord& record)
{
	WORD cce;
	record >> cce;
	rgce.load(record, cce);
}


} // namespace XLS

