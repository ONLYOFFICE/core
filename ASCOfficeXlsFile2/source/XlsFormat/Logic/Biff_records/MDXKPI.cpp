
#include "MDXKPI.h"

namespace XLS
{;

MDXKPI::MDXKPI()
{
}


MDXKPI::~MDXKPI()
{
}


BaseObjectPtr MDXKPI::clone()
{
	return BaseObjectPtr(new MDXKPI(*this));
}


void MDXKPI::writeFields(CFRecord& record)
{
#pragma message("####################### MDXKPI record is not implemented")
	Log::error("MDXKPI record is not implemented.");
	//record << some_value;
}


void MDXKPI::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	record >> istrConnName >> tfnSrc >> kpiprop >> istrKPIName >> istrMbrKPI;
}

} // namespace XLS

