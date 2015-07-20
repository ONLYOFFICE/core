
#include "CFParsedFormula.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


CFParsedFormula::CFParsedFormula(const CellRef& cell_base_ref)
:	ParsedFormula(cell_base_ref)
{
}


BiffStructurePtr CFParsedFormula::clone()
{
	return BiffStructurePtr(new CFParsedFormula(*this));
}


void CFParsedFormula::store(CFRecord& record)
{
#pragma message("####################### CFParsedFormula record has no BiffStructure::store() implemented")
	Log::error(" Error!!! CFParsedFormula record has no BiffStructure::store() implemented.");
	//record << something;
}


void CFParsedFormula::load(CFRecord& record)
{
	unsigned short cce;
	record >> cce;
	rgce.load(record, cce);
}


} // namespace XLS

