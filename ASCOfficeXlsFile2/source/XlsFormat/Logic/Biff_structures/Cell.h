#pragma once

#include "BiffStructure.h"
#include "CellRef.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class Cell : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Cell)
public:
	Cell();
	Cell(const unsigned __int16 rw_init, const unsigned __int16 col_init, const unsigned __int16 ixfe_init);

	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
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
