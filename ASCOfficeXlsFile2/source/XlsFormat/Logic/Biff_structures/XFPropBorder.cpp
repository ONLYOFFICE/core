
#include "XFPropBorder.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr XFPropBorder::clone()
{
	return BiffStructurePtr(new XFPropBorder(*this));
}

void XFPropBorder::store(CFRecord& record)
{
	record << color << dgBorder;
}


void XFPropBorder::load(CFRecord& record)
{
	record >> color >> dgBorder;
}


} // namespace XLS
