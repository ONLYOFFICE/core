
#include "Protect.h"

namespace XLS
{

Protect::Protect()
{
}


Protect::~Protect()
{
}


BaseObjectPtr Protect::clone()
{
	return BaseObjectPtr(new Protect(*this));
}


void Protect::writeFields(CFRecord& record)
{
	record << fLock;
}


void Protect::readFields(CFRecord& record)
{
	record >> fLock;
}

} // namespace XLS

