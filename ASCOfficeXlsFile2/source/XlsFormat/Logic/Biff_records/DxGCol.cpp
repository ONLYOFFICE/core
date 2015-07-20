
#include "DxGCol.h"

namespace XLS
{;

DxGCol::DxGCol()
{
}


DxGCol::~DxGCol()
{
}


BaseObjectPtr DxGCol::clone()
{
	return BaseObjectPtr(new DxGCol(*this));
}


void DxGCol::writeFields(CFRecord& record)
{
	record << dxgCol;
}


void DxGCol::readFields(CFRecord& record)
{
	record >> dxgCol;
}

} // namespace XLS

