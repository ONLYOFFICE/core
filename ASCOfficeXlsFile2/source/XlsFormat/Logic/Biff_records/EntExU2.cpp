#include "precompiled_xls.h"
#include "EntExU2.h"
//#include <Exception/UnexpectedProgramPath.h>

namespace XLS
{;

EntExU2::EntExU2()
{
}


EntExU2::~EntExU2()
{
}


BaseObjectPtr EntExU2::clone()
{
	return BaseObjectPtr(new EntExU2(*this));
}


void EntExU2::writeFields(CFRecord& record)
{
	throw;// EXCEPT::LE::UnexpectedProgramPath("EntExU2 record is skipped.", __FUNCTION__);
}


void EntExU2::readFields(CFRecord& record)
{
	record.skipNunBytes(record.getDataSize()); // SHOULD NOT be written, and SHOULD be ignored.
}

} // namespace XLS

