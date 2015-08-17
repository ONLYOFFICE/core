
#include "ArrayParsedFormula.h"
#include <Binary/CFRecord.h>
//#include <Exception/UnexpectedProgramPath.h>

namespace XLS
{

ArrayParsedFormula::ArrayParsedFormula(const bool is_part_of_a_revision, const CellRef& cell_base_ref)
:	is_part_of_a_revision_(is_part_of_a_revision),
	ParsedFormula(cell_base_ref)
{

}


BiffStructurePtr ArrayParsedFormula::clone()
{
	return BiffStructurePtr(new ArrayParsedFormula(*this));
}


void ArrayParsedFormula::load(CFRecord& record)
{
	unsigned short cce;
	record >> cce;
	rgce.load(record, cce);
	rgcb.load(record, rgce.getPtgs(), is_part_of_a_revision_);
}


void ArrayParsedFormula::store(CFRecord& record)
{
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cce*/);
	rgce.store(record);
	record.registerDelayedDataSource(rgce.getCCE(), rt_Array);
	rgcb.store(record);
}


} // namespace XLS

