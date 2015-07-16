#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/SqRefU.h>

namespace XLS
{;

class CFRecord;

class CondFmtStructure : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CondFmtStructure)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const CellRef getLocation() const;

private:
	unsigned short ccf;
	bool fToughRecalc;
	unsigned short nID;
	Ref8U refBound;
	SqRefU sqref;
};

} // namespace XLS

