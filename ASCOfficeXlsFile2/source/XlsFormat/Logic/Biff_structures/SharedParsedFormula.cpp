
#include "SharedParsedFormula.h"
#include <Binary/CFRecord.h>
//#include <Exception/UnexpectedProgramPath.h>

namespace XLS
{

SharedParsedFormula::SharedParsedFormula(const bool is_part_of_a_revision, const CellRef& cell_base_ref)
:	is_part_of_a_revision_(is_part_of_a_revision),
	ParsedFormula(cell_base_ref)
{

}

BiffStructurePtr SharedParsedFormula::clone()
{
	return BiffStructurePtr(new SharedParsedFormula(*this));
}


void SharedParsedFormula::load(CFRecord& record)
{
	unsigned short cce;
	record >> cce;
	
	rgce.load(record, cce);
	rgcb.load(record, rgce.getPtgs(), is_part_of_a_revision_);
}


void SharedParsedFormula::store(CFRecord& record)
{
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cce*/);
	rgce.store(record);
	record.registerDelayedDataSource(rgce.getCCE(), rt_ShrFmla);
	rgcb.store(record);
}


} // namespace XLS

