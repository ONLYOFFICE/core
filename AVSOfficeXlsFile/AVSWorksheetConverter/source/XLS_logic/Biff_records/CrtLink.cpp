#include "stdafx.h"
#include "CrtLink.h"

namespace XLS
{;

CrtLink::CrtLink()
{
}


CrtLink::~CrtLink()
{
}


BaseObjectPtr CrtLink::clone()
{
	return BaseObjectPtr(new CrtLink(*this));
}


void CrtLink::writeFields(CFRecord& record)
{
	record.reserveNBytes(10); //unused
}


void CrtLink::readFields(CFRecord& record)
{
	record.skipNBytes(10); //unused
}

} // namespace XLS

