#include "precompiled_xls.h"
#include "CellParsedFormula.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


CellParsedFormula::CellParsedFormula(const bool is_part_of_a_revision)
:	is_part_of_a_revision_(is_part_of_a_revision),
	ParsedFormula(CellRef())
{
}


BiffStructurePtr CellParsedFormula::clone()
{
	return BiffStructurePtr(new CellParsedFormula(*this));
}


void CellParsedFormula::store(CFRecord& record)
{
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cce*/);
	rgce.store(record);
	record.registerDelayedDataSource(rgce.getCCE(), rt_Formula);
	rgcb.store(record);
}


void CellParsedFormula::load(CFRecord& record)
{
	unsigned short cce;
	record >> cce;
	rgce.load(record, cce);
	rgcb.load(record, rgce.getPtgs(), is_part_of_a_revision_);
}


} // namespace XLS

