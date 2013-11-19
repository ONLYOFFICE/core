#include "stdafx.h"
#include "Pane.h"

namespace XLS
{;

Pane::Pane()
{
}


Pane::~Pane()
{
}


BaseObjectPtr Pane::clone()
{
	return BaseObjectPtr(new Pane(*this));
}


void Pane::writeFields(CFRecord& record)
{
	record << x << y;
	CellRef topLeftCell_ref(topLeftCell);
	rwTop = static_cast<WORD>(topLeftCell_ref.getRow());
	colLeft = static_cast<WORD>(topLeftCell_ref.getColumn());
	record << rwTop << colLeft << pnnAcct;
	record.reserveNBytes(1); // reserved
}


void Pane::readFields(CFRecord& record)
{
	record >> x >> y >> rwTop >> colLeft >> pnnAcct;
	topLeftCell = static_cast<_bstr_t>(CellRef(rwTop, colLeft, true, true));
	record.skipNBytes(1); // reserved
}

} // namespace XLS

