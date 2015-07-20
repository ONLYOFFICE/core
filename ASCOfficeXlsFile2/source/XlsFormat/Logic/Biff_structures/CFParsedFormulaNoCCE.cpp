
#include "CFParsedFormulaNoCCE.h"
#include <Binary/CFRecord.h>
//#include <Exception/UnexpectedProgramPath.h>

namespace XLS
{;


CFParsedFormulaNoCCE::CFParsedFormulaNoCCE(const CellRef& cell_base_ref_init)
:	ParsedFormula(cell_base_ref_init)
{
}


BiffStructurePtr CFParsedFormulaNoCCE::clone()
{
	return BiffStructurePtr(new CFParsedFormulaNoCCE(*this));
}


void CFParsedFormulaNoCCE::store(CFRecord& record)
{
	rgce.store(record);
}


void CFParsedFormulaNoCCE::load(CFRecord& record)
{
	throw;// EXCEPT::LE::WhatIsTheFuck("Unexpected stub function call.", __FUNCTION__);
}


void CFParsedFormulaNoCCE::load(CFRecord& record, const unsigned short cce)
{
	rgce.load(record, cce);
}


} // namespace XLS

