#include "precompiled_xls.h"
#include "ChartParsedFormula.h"
#include <Binary/CFRecord.h>

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
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned __int16)/*cce*/);
	rgce.store(record);
	record.registerDelayedDataSource(rgce.getCCE(), rt_Formula);
}


void ChartParsedFormula::load(CFRecord& record)
{
	unsigned __int16 cce;
	record >> cce;
	rgce.load(record, cce);
}


} // namespace XLS

