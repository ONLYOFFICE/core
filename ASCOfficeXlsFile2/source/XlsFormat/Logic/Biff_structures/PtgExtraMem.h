#pragma once

#include "Ptg.h"
#include "CellRangeRef.h"

namespace XLS
{;

class CFRecord;

class PtgExtraMem : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgExtraMem)
public:
	PtgExtraMem();
	PtgExtraMem(CFRecord& record);
	BiffStructurePtr clone();

	
	virtual void load(CFRecord& record); // No type info
	virtual void store(CFRecord& record); // No type info

	const std::wstring toString() const;
private:
	std::vector<Ref8U> array_;
};

typedef boost::shared_ptr<PtgExtraMem> PtgExtraMemPtr;


} // namespace XLS

