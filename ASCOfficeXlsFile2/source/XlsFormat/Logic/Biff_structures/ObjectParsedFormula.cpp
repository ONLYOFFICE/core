
#include "ObjectParsedFormula.h"
#include <Binary/CFRecord.h>

namespace XLS
{


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
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cce*/);
	record.reserveNunBytes(4); // unused
	rgce.store(record);
	record.registerDelayedDataSource(rgce.getCCE(), rt_Obj);
}


void ObjectParsedFormula::load(CFRecord& record)
{
	unsigned short cce;
	record >> cce;
	cce = GETBITS(cce, 0, 14);

	record.skipNunBytes(4); // unused
	rgce.load(record, cce);
}



} // namespace XLS

