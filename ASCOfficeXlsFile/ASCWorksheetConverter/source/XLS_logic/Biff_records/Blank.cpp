#include "stdafx.h"
#include "Blank.h"

namespace XLS
{;

Blank::Blank()
{
}


Blank::~Blank()
{
}


BaseObjectPtr Blank::clone()
{
	return BaseObjectPtr(new Blank(*this));
}


void Blank::writeFields(CFRecord& record)
{
	resender.store(record);
	record << cell;
}


void Blank::readFields(CFRecord& record)
{
	record >> cell;
}

const CellRef Blank::getLocation() const
{
	return cell.getLocation();
}

} // namespace XLS

