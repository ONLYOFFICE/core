#include "stdafx.h"
#include "SerAuxErrBar.h"

namespace XLS
{;

SerAuxErrBar::SerAuxErrBar()
{
}


SerAuxErrBar::~SerAuxErrBar()
{
}


BaseObjectPtr SerAuxErrBar::clone()
{
	return BaseObjectPtr(new SerAuxErrBar(*this));
}


void SerAuxErrBar::writeFields(CFRecord& record)
{
	record << sertm << ebsrc << fTeeTop;
	record.reserveNBytes(1); // reserved
	record << numValue << cnum;
}


void SerAuxErrBar::readFields(CFRecord& record)
{
	record >> sertm >> ebsrc >> fTeeTop;
	record.skipNBytes(1); // reserved
	record >> numValue >> cnum;
}

} // namespace XLS

