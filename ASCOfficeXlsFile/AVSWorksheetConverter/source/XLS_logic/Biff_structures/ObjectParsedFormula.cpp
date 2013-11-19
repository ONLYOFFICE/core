#include "stdafx.h"
#include "ObjectParsedFormula.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


ObjectParsedFormula::ObjectParsedFormula()
:	ParsedFormula(CellRef())
{
}


BiffStructurePtr ObjectParsedFormula::clone()
{
	return BiffStructurePtr(new ObjectParsedFormula(*this));
}


void ObjectParsedFormula::store(CFRecord& record)
{
	record.registerDelayedDataReceiver(NULL, sizeof(WORD)/*cce*/);
	record.reserveNBytes(4); // unused
	rgce.store(record);
	record.registerDelayedDataSource(rgce.getCCE(), rt_Obj);
}


void ObjectParsedFormula::load(CFRecord& record)
{
	WORD cce;
	record >> cce;
	cce = GETBITS(cce, 0, 14);

	record.skipNBytes(4); // unused
	rgce.load(record, cce);
}



} // namespace XLS

