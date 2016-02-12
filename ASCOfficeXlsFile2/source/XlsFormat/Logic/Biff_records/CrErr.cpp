
#include "CrErr.h"

namespace XLS
{

CrErr::CrErr()
{
}


CrErr::~CrErr()
{
}


BaseObjectPtr CrErr::clone()
{
	return BaseObjectPtr(new CrErr(*this));
}


void CrErr::writeFields(CFRecord& record)
{

}


void CrErr::readFields(CFRecord& record)
{
#pragma message("####################### CrErr record is not implemented")
	Log::error("CrErr record is not implemented.");

	record.skipNunBytes(record.getDataSize() - record.getRdPtr());
}

} // namespace XLS

