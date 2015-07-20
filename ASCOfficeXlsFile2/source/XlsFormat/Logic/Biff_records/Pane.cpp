
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
	rwTop = static_cast<unsigned short>(topLeftCell_ref.getRow());
	colLeft = static_cast<unsigned short>(topLeftCell_ref.getColumn());
	record << rwTop << colLeft << pnnAcct;
	record.reserveNunBytes(1); // reserved
}


void Pane::readFields(CFRecord& record)
{
	record >> x >> y >> rwTop >> colLeft >> pnnAcct;
	topLeftCell = static_cast<std::wstring >(CellRef(rwTop, colLeft, true, true));
	record.skipNunBytes(1); // reserved
}

} // namespace XLS

