
#include "ShrFmla.h"

namespace XLS
{;

ShrFmla::ShrFmla(const CellRef& cell_base_ref_init)
:	formula(false, cell_base_ref_init)
{
}


ShrFmla::~ShrFmla()
{
}


BaseObjectPtr ShrFmla::clone()
{
	return BaseObjectPtr(new ShrFmla(*this));
}


void ShrFmla::writeFields(CFRecord& record)
{
	record << ref_;
	record.reserveNunBytes(1); // reserved
	record << cUse;
	formula.store(record);
}


void ShrFmla::readFields(CFRecord& record)
{
	record >> ref_;
	record.skipNunBytes(1); // reserved
	record >> cUse;
	formula.load(record);
}

} // namespace XLS

