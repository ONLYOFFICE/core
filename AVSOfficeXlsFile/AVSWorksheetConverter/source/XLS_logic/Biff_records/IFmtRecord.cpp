#include "stdafx.h"
#include "IFmtRecord.h"

namespace XLS
{;

IFmtRecord::IFmtRecord()
{
}


IFmtRecord::~IFmtRecord()
{
}


BaseObjectPtr IFmtRecord::clone()
{
	return BaseObjectPtr(new IFmtRecord(*this));
}


void IFmtRecord::writeFields(CFRecord& record)
{
	record << ifmt;
}


void IFmtRecord::readFields(CFRecord& record)
{
	record >> ifmt;
}

} // namespace XLS

