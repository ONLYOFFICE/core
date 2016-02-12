
#include "FrtWrapper.h"

namespace XLS
{

FrtWrapper::FrtWrapper()
{
}


FrtWrapper::~FrtWrapper()
{
}


BaseObjectPtr FrtWrapper::clone()
{
	return BaseObjectPtr(new FrtWrapper(*this));
}


void FrtWrapper::writeFields(CFRecord& record)
{

}


void FrtWrapper::readFields(CFRecord& record)
{
#pragma message("####################### FrtWrapper record is not implemented")
	Log::error("FrtWrapper record is not implemented.");
	
	record.skipNunBytes(record.getDataSize() - record.getRdPtr());

}

} // namespace XLS

