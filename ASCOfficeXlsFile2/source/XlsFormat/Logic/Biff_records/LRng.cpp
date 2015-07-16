#include "precompiled_xls.h"
#include "LRng.h"

namespace XLS
{;

LRng::LRng()
{
}


LRng::~LRng()
{
}


BaseObjectPtr LRng::clone()
{
	return BaseObjectPtr(new LRng(*this));
}


void LRng::writeFields(CFRecord& record)
{
#pragma message("####################### LRng record is not implemented")
	Log::error("LRng record is not implemented.");
	//record << some_value;
}


void LRng::readFields(CFRecord& record)
{
	record >> crefRw;
	for(unsigned short i = 0; i < crefRw; ++i)
	{
		Ref8U cell_ref;
		record >> cell_ref;
		refRow.push_back(CellRangeRefPtr(new Ref8U(cell_ref)));
	}
	record >> crefCol;
	for(unsigned short i = 0; i < crefCol; ++i)
	{
		Ref8U cell_ref;
		record >> cell_ref;
		refCol.push_back(CellRangeRefPtr(new Ref8U(cell_ref)));
	}

}

} // namespace XLS

