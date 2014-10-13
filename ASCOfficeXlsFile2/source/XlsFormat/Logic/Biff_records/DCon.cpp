#include "precompiled_xls.h"
#include "DCon.h"

namespace XLS
{;

DCon::DCon()
{
}


DCon::~DCon()
{
}


BaseObjectPtr DCon::clone()
{
	return BaseObjectPtr(new DCon(*this));
}


void DCon::writeFields(CFRecord& record)
{
	#pragma message("####################### DCon record is not implemented")
	//record << iiftab << fLeftCat << fTopCat << fLinkConsole;
}


void DCon::readFields(CFRecord& record)
{
	#pragma message("####################### DCon record is not implemented")
	//record >> iiftab >> fLeftCat >> fTopCat >> fLinkConsole;
}

} // namespace XLS

