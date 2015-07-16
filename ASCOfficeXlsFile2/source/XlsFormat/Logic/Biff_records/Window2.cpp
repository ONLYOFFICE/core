#include "precompiled_xls.h"
#include "Window2.h"

namespace XLS
{;

Window2::Window2()
{
}


Window2::~Window2()
{
}


BaseObjectPtr Window2::clone()
{
	return BaseObjectPtr(new Window2(*this));
}


void Window2::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 9, fSelected);

	if(is_contained_in_chart_substream)
	{
		record << flags;
		record.reserveNunBytes(8); // must be ignored
		return;
	}

	SETBIT(flags, 0, fDspFmlaRt);
	SETBIT(flags, 1, fDspGridRt);
	SETBIT(flags, 2, fDspRwColRt);
	SETBIT(flags, 3, fFrozenRt);
	SETBIT(flags, 4, fDspZerosRt);
	SETBIT(flags, 5, fDefaultHdr);
	SETBIT(flags, 6, fRightToLeft);
	SETBIT(flags, 7, fDspGuts);
	SETBIT(flags, 8, fFrozenNoSplit);
	SETBIT(flags, 10, fPaged);
	SETBIT(flags, 11, fSLV);
	record << flags;

	CellRef topLeftCellRef(topLeftCell);
	rwTop = static_cast<unsigned short>(topLeftCellRef.getRow());
	colLeft = static_cast<unsigned short>(topLeftCellRef.getColumn());
	record << rwTop << colLeft << icvHdr;

	record.reserveNunBytes(2); // reserved 
	record << wScaleSLV << wScaleNormal;
	record.reserveNunBytes(4); // unused / reserved
}


void Window2::readFields(CFRecord& record)
{
	is_contained_in_chart_substream = (10 == record.getDataSize());
	unsigned short flags;
	record >> flags;

	fSelected = GETBIT(flags, 9);
	if(is_contained_in_chart_substream)
	{
		record.skipNunBytes(8); // must be ignored
		return;
	}

	fDspFmlaRt = GETBIT(flags, 0);
	fDspGridRt = GETBIT(flags, 1);
	fDspRwColRt = GETBIT(flags, 2);
	fFrozenRt = GETBIT(flags, 3);
	fDspZerosRt = GETBIT(flags, 4);
	fDefaultHdr = GETBIT(flags, 5);
	fRightToLeft = GETBIT(flags, 6);
	fDspGuts = GETBIT(flags, 7);
	fFrozenNoSplit = GETBIT(flags, 8);
	fPaged = GETBIT(flags, 10);
	fSLV = GETBIT(flags, 11);

	record >> rwTop >> colLeft >> icvHdr;
	topLeftCell = static_cast<std::wstring >(CellRef(rwTop, colLeft, true, true));
	record.skipNunBytes(2); // reserved
	record >> wScaleSLV >> wScaleNormal;
	record.skipNunBytes(4); // unused / reserved
}

} // namespace XLS

