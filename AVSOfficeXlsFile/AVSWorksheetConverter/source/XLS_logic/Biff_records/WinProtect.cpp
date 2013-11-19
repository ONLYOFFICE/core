#include "stdafx.h"
#include "WinProtect.h"

namespace XLS
{;

WinProtect::WinProtect()
{
}


WinProtect::~WinProtect()
{
}


BaseObjectPtr WinProtect::clone()
{
	return BaseObjectPtr(new WinProtect(*this));
}


void WinProtect::writeFields(CFRecord& record)
{
	record << fLockWn;
}


void WinProtect::readFields(CFRecord& record)
{
	record >> fLockWn;
}

} // namespace XLS

