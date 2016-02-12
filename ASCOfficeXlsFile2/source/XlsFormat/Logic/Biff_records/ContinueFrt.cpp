
#include "ContinueFrt.h"

namespace XLS
{

ContinueFrt::ContinueFrt()
{
}


ContinueFrt::~ContinueFrt()
{
}


BaseObjectPtr ContinueFrt::clone()
{
	return BaseObjectPtr(new ContinueFrt(*this));
}


void ContinueFrt::writeFields(CFRecord& record)
{

}


void ContinueFrt::readFields(CFRecord& record)
{
#pragma message("####################### ContinueFrt record is not implemented")
	Log::error("ContinueFrt record is not implemented.");
	
	record.skipNunBytes(record.getDataSize() - record.getRdPtr());
}

} // namespace XLS

