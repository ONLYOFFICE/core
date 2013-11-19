#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/SqRefU.h>

namespace XLS
{;

class CFRecord;

class CondFmtStructure : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CondFmtStructure)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const CellRef getLocation() const;

private:
	WORD ccf;
	bool fToughRecalc;
	WORD nID;
	Ref8U refBound;
	SqRefU sqref;
};

} // namespace XLS

