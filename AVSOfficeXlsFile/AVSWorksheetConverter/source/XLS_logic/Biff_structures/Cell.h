#pragma once

#include "BiffStructTagged.h"
#include "CellRef.h"
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class Cell : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Cell)
public:
	Cell();
	Cell(const WORD rw_init, const WORD col_init, const WORD ixfe_init);

	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const CellRef getLocation() const;

private:
	Rw rw;
	Col col;
	IXFCell ixfe;
};

typedef boost::shared_ptr<Cell> CellPtr;

} // namespace XLS
