
#include "NameParsedFormula.h"
#include <Binary/CFRecord.h>

namespace XLS
{;

NameParsedFormula::NameParsedFormula(const bool is_part_of_a_revision)
:	is_part_of_a_revision_(is_part_of_a_revision),
	ParsedFormula(CellRef())
{

}


BiffStructurePtr NameParsedFormula::clone()
{
	return BiffStructurePtr(new NameParsedFormula(*this));
}


void NameParsedFormula::store(CFRecord& record)
{
	rgce.store(record);
	rgcb.store(record);
}


void NameParsedFormula::load(CFRecord& record, const size_t cce)
{
	rgce.load(record, cce);
	rgcb.load(record, rgce.getPtgs(), is_part_of_a_revision_);
}


} // namespace XLS

