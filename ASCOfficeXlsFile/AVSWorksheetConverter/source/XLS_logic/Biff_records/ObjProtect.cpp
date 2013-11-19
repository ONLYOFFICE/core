#include "stdafx.h"
#include "ObjProtect.h"

namespace XLS
{;

ObjProtect::ObjProtect()
{
}


ObjProtect::~ObjProtect()
{
}


BaseObjectPtr ObjProtect::clone()
{
	return BaseObjectPtr(new ObjProtect(*this));
}


void ObjProtect::writeFields(CFRecord& record)
{
	record << fLockObj;
}


void ObjProtect::readFields(CFRecord& record)
{
	record >> fLockObj;
}

} // namespace XLS

