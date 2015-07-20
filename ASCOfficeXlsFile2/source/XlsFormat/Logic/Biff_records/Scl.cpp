
#include "Scl.h"

namespace XLS
{;

Scl::Scl()
{
}


Scl::~Scl()
{
}


BaseObjectPtr Scl::clone()
{
	return BaseObjectPtr(new Scl(*this));
}


void Scl::writeFields(CFRecord& record)
{
	record << nscl << dscl;
}


void Scl::readFields(CFRecord& record)
{
	record >> nscl >> dscl;
	//Fraction = static_cast<double>(nscl) / static_cast<double>(dscl);
}

} // namespace XLS

