#include "stdafx.h"
#include "CFVOParsedFormula.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


CFVOParsedFormula::CFVOParsedFormula()
:	ParsedFormula(CellRef())
{
}


BiffStructurePtr CFVOParsedFormula::clone()
{
	return BiffStructurePtr(new CFVOParsedFormula(*this));
}


void CFVOParsedFormula::store(CFRecord& record)
{
	record.registerDelayedDataReceiver(NULL, sizeof(WORD)/*cce*/, rt_STRUCT_CFVOParsedFormula);
	rgce.store(record);
	record.registerDelayedDataSource(rgce.getCCE(), rt_STRUCT_CFVOParsedFormula);
}


void CFVOParsedFormula::load(CFRecord& record)
{
	WORD cce;
	record >> cce;
	rgce.load(record, cce);
}


} // namespace XLS

