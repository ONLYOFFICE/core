
#include "CrtLine.h"

namespace XLS
{;

CrtLine::CrtLine()
{
}


CrtLine::~CrtLine()
{
}


BaseObjectPtr CrtLine::clone()
{
	return BaseObjectPtr(new CrtLine(*this));
}


void CrtLine::writeFields(CFRecord& record)
{
	record << id;
}


void CrtLine::readFields(CFRecord& record)
{
	record >> id;
}

} // namespace XLS

