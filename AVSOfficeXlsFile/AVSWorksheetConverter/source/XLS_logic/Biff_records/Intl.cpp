#include "stdafx.h"
#include "Intl.h"

namespace XLS
{;

Intl::Intl()
{
}


Intl::~Intl()
{
}


BaseObjectPtr Intl::clone()
{
	return BaseObjectPtr(new Intl(*this));
}


void Intl::writeFields(CFRecord& record)
{
	record.reserveNBytes(2); // reserved
}


void Intl::readFields(CFRecord& record)
{
	record.skipNBytes(2); // reserved
}

} // namespace XLS

