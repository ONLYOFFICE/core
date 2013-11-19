#include "stdafx.h"
#include "Uncalced.h"

namespace XLS
{;

Uncalced::Uncalced()
{
}


Uncalced::~Uncalced()
{
}


BaseObjectPtr Uncalced::clone()
{
	return BaseObjectPtr(new Uncalced(*this));
}


void Uncalced::writeFields(CFRecord& record)
{
	record.reserveNBytes(2); // reserved
}


void Uncalced::readFields(CFRecord& record)
{
	record.skipNBytes(2); // reserved
}

} // namespace XLS

