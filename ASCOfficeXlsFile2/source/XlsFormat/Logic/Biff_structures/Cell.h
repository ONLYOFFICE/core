#pragma once

#include "BiffStructure.h"
#include "CellRef.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class Cell : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Cell)
public:
	Cell();
	Cell(const unsigned short rw_init, const unsigned short col_init, const unsigned short ixfe_init);

	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const CellRef getLocation() const;

	Rw rw;
	Col col;
	IXFCell ixfe;
};

typedef boost::shared_ptr<Cell> CellPtr;

} // namespace XLS
