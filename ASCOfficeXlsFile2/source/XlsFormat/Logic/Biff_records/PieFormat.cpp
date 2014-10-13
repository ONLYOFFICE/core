#include "precompiled_xls.h"
#include "PieFormat.h"

namespace XLS
{;

PieFormat::PieFormat()
{
}


PieFormat::~PieFormat()
{
}


BaseObjectPtr PieFormat::clone()
{
	return BaseObjectPtr(new PieFormat(*this));
}


void PieFormat::writeFields(CFRecord& record)
{
	record << pcExplode;
}


void PieFormat::readFields(CFRecord& record)
{
	record >> pcExplode;
}

} // namespace XLS

