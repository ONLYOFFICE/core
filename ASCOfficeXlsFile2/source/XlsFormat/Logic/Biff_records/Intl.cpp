#include "precompiled_xls.h"
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
	record.reserveNunBytes(2); // reserved
}


void Intl::readFields(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
}

} // namespace XLS

