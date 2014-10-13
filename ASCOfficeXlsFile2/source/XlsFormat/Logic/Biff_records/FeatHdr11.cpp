#include "precompiled_xls.h"
#include "FeatHdr11.h"

namespace XLS
{;

FeatHdr11::FeatHdr11()
{
}


FeatHdr11::~FeatHdr11()
{
}


BaseObjectPtr FeatHdr11::clone()
{
	return BaseObjectPtr(new FeatHdr11(*this));
}


void FeatHdr11::writeFields(CFRecord& record)
{
}


void FeatHdr11::readFields(CFRecord& record)
{
	record >> frt;
	record >> isf;
	record.skipNunBytes(1);		// reserved1
	record.skipNunBytes(4);		// reserved2
	record.skipNunBytes(4);		// reserved3
	record >> idListNext;
	record.skipNunBytes(2);		// reserved4
}

} // namespace XLS

