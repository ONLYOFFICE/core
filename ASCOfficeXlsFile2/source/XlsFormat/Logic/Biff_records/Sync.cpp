#include "precompiled_xls.h"
#include "Sync.h"
#include <Logic/Biff_structures/CellRef.h>

namespace XLS
{;

Sync::Sync()
{
}


Sync::~Sync()
{
}


BaseObjectPtr Sync::clone()
{
	return BaseObjectPtr(new Sync(*this));
}


void Sync::writeFields(CFRecord& record)
{
	CellRef ref_temp(ref_);
	rw = static_cast<unsigned short>(ref_temp.getRow());
	col = static_cast<unsigned short>(ref_temp.getColumn());
	record << rw << col;
}


void Sync::readFields(CFRecord& record)
{
	record >> rw >> col;
	ref_ = std::wstring (CellRef(rw, col, true, true).toString().c_str());
}

} // namespace XLS

