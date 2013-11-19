#include "stdafx.h"
#include "MulRk.h"

namespace XLS
{;

MulRk::MulRk()
{
}


MulRk::~MulRk()
{
}


BaseObjectPtr MulRk::clone()
{
	return BaseObjectPtr(new MulRk(*this));
}


void MulRk::writeFields(CFRecord& record)
{
#pragma message("####################### MulRk record is not implemented")
	Log::error("MulRk record is not implemented.");
	//record << some_value;
}


void MulRk::readFields(CFRecord& record)
{
	Col colFirst;
	Col colLast;
// A little hack to extract colLast before it is used
	record.skipNBytes(record.getDataSize() - sizeof(WORD));
	record >> colLast;
	record.resetPointerToBegin();
//------------------
	record >> rw >> colFirst;
	for(WORD i = colFirst; i <= colLast; ++i)
	{
		RkRec rec;
		record >> rec;
		rgrkrec.push_back(RkRecPtr(new RkRec(rec)));
		cells.push_back(CellPtr(new Cell(rw, i, rec.get_ixfe())));
	}

	record.skipNBytes(sizeof(WORD));
}

const long MulRk::GetRow() const
{
	return static_cast<WORD>(rw);
}

} // namespace XLS

