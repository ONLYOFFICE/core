#include "stdafx.h"
#include "RK.h"

namespace XLS
{;

RK::RK()
{
}


RK::~RK()
{
}


BaseObjectPtr RK::clone()
{
	return BaseObjectPtr(new RK(*this));
}


void RK::writeFields(CFRecord& record)
{
#pragma message("####################### RK record is not implemented")
	Log::error("RK record is not implemented.");
	//record << some_value;
}


void RK::readFields(CFRecord& record)
{
	Rw rw;
	Col col;
	record >> rw >> col >> rkrec;
	cell = Cell(rw, col, rkrec.get_ixfe());
}

const CellRef RK::getLocation() const
{
	return cell.getLocation();
}

} // namespace XLS

