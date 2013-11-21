#include "stdafx.h"
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
	record.skipNBytes(1);		// reserved1
	record.skipNBytes(4);		// reserved2
	record.skipNBytes(4);		// reserved3
	record >> idListNext;
	record.skipNBytes(2);		// reserved4
}

} // namespace XLS

