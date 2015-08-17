
#include "PrintGrid.h"

namespace XLS
{

PrintGrid::PrintGrid()
{
}


PrintGrid::~PrintGrid()
{
}


BaseObjectPtr PrintGrid::clone()
{
	return BaseObjectPtr(new PrintGrid(*this));
}


void PrintGrid::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fPrintGrid);
	record << flags;
}


void PrintGrid::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fPrintGrid = GETBIT(flags, 0);
}

} // namespace XLS

