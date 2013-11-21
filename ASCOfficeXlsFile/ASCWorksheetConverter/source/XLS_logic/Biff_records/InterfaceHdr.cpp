#include "stdafx.h"
#include "InterfaceHdr.h"
#include <XLS_bin/CFStream.h>

namespace XLS
{;


InterfaceHdr::InterfaceHdr()
{
}


InterfaceHdr::~InterfaceHdr()
{
}


BaseObjectPtr InterfaceHdr::clone()
{
	return BaseObjectPtr(new InterfaceHdr(*this));
}


void InterfaceHdr::writeFields(CFRecord& record)
{
	record << codePage;
}


void InterfaceHdr::readFields(CFRecord& record)
{
	record >> codePage;
}

} // namespace XLS
