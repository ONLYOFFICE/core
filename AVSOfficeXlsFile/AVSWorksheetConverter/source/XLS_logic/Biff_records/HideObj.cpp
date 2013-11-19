#include "stdafx.h"
#include "HideObj.h"

namespace XLS
{;

HideObj::HideObj()
{
}


HideObj::~HideObj()
{
}


BaseObjectPtr HideObj::clone()
{
	return BaseObjectPtr(new HideObj(*this));
}


void HideObj::writeFields(CFRecord& record)
{
	record << hideObj;
}


void HideObj::readFields(CFRecord& record)
{
	record >> hideObj;
}

} // namespace XLS

