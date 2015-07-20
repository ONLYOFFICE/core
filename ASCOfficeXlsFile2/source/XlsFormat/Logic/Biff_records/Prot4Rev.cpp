
#include "Prot4Rev.h"

namespace XLS
{;

Prot4Rev::Prot4Rev()
{
}


Prot4Rev::~Prot4Rev()
{
}


BaseObjectPtr Prot4Rev::clone()
{
	return BaseObjectPtr(new Prot4Rev(*this));
}


void Prot4Rev::writeFields(CFRecord& record)
{
	record << fRevLock;
}


void Prot4Rev::readFields(CFRecord& record)
{
	record >> fRevLock;
}

} // namespace XLS

