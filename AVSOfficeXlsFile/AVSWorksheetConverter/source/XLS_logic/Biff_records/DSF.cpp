#include "stdafx.h"
#include "DSF.h"

namespace XLS
{;

DSF::DSF()
{
}


DSF::~DSF()
{
}


BaseObjectPtr DSF::clone()
{
	return BaseObjectPtr(new DSF(*this));
}


void DSF::writeFields(CFRecord& record)
{
	// the tag is reserved and all the data is reserved
	record.reserveNBytes(2); // reserved
}


void DSF::readFields(CFRecord& record)
{
	// the tag is reserved and all the data is reserved
	record.skipNBytes(2); // reserved
}

} // namespace XLS

