#include "precompiled_xls.h"
#include "BRAI.h"

namespace XLS
{;

BRAI::BRAI()
{
}


BRAI::~BRAI()
{
}


BaseObjectPtr BRAI::clone()
{
	return BaseObjectPtr(new BRAI(*this));
}


void BRAI::writeFields(CFRecord& record)
{
	unsigned __int16 flags = 0;
	SETBIT(flags, 0, fUnlinkedIfmt);
	record << id << rt << flags << ifmt;
	formula.store(record);
}


void BRAI::readFields(CFRecord& record)
{
	unsigned __int16 flags;
	record >> id >> rt >> flags;
	fUnlinkedIfmt = static_cast<unsigned char>(GETBIT(flags, 0));
	record >> ifmt;
	formula.load(record);
}

} // namespace XLS

