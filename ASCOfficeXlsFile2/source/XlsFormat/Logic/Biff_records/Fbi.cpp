
#include "Fbi.h"

namespace XLS
{

Fbi::Fbi()
{
}


Fbi::~Fbi()
{
}


BaseObjectPtr Fbi::clone()
{
	return BaseObjectPtr(new Fbi(*this));
}


void Fbi::writeFields(CFRecord& record)
{
	record << dmixBasis << dmiyBasis << twpHeightBasis << scab << ifnt;
}


void Fbi::readFields(CFRecord& record)
{
	record >> dmixBasis >> dmiyBasis >> twpHeightBasis >> scab >> ifnt;
}

} // namespace XLS

