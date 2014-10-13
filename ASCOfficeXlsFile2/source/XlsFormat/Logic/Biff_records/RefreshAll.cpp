#include "precompiled_xls.h"
#include "RefreshAll.h"

namespace XLS
{;

RefreshAll::RefreshAll()
{
}


RefreshAll::~RefreshAll()
{
}


BaseObjectPtr RefreshAll::clone()
{
	return BaseObjectPtr(new RefreshAll(*this));
}


void RefreshAll::writeFields(CFRecord& record)
{
	record << refreshAll;
}


void RefreshAll::readFields(CFRecord& record)
{
	record >> refreshAll;
}

} // namespace XLS

