#pragma once

#include "BiffStructure.h"
#include "SqRefU.h"

namespace XLS
{

class CFRecord;

class CondFmtStructure : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CondFmtStructure)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeCondFmtStructure;
	
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

