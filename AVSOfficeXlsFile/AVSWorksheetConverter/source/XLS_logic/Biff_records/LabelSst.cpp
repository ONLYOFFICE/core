#include "stdafx.h"
#include "LabelSst.h"

namespace XLS
{;

LabelSst::LabelSst()
{
}


LabelSst::~LabelSst()
{
}


BaseObjectPtr LabelSst::clone()
{
	return BaseObjectPtr(new LabelSst(*this));
}


void LabelSst::writeFields(CFRecord& record)
{
	resender.store(record);
	record << cell << isst;
}


void LabelSst::readFields(CFRecord& record)
{
	record >> cell >> isst;
}

const CellRef LabelSst::getLocation() const
{
	return cell.getLocation();
}

} // namespace XLS

