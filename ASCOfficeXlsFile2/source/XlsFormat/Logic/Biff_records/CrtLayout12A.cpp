
#include "CrtLayout12A.h"

namespace XLS
{

CrtLayout12A::CrtLayout12A()
{
}


CrtLayout12A::~CrtLayout12A()
{
}


BaseObjectPtr CrtLayout12A::clone()
{
	return BaseObjectPtr(new CrtLayout12A(*this));
}


void CrtLayout12A::writeFields(CFRecord& record)
{

}


void CrtLayout12A::readFields(CFRecord& record)
{
#pragma message("####################### CrtLayout12A record is not implemented")
	Log::error("CrtLayout12A record is not implemented.");

	record.skipNunBytes(record.getDataSize() - record.getRdPtr());
}

} // namespace XLS

