
#include "Cell.h"
#include "CellRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{


Cell::Cell()
{
}


Cell::Cell(const unsigned short rw_init, const unsigned short col_init, const unsigned short ixfe_init)
{
	rw = rw_init;
	col = col_init;
	ixfe = ixfe_init;
}


BiffStructurePtr Cell::clone()
{
	return BiffStructurePtr(new Cell(*this));
}


void Cell::store(CFRecord& record)
{
	record << rw << col << ixfe;
}


void Cell::load(CFRecord& record)
{
	record >> rw >> col >> ixfe;
}


const CellRef Cell::getLocation() const
{
	return RgceLoc(rw, col, true, true);
}


} // namespace XLS
