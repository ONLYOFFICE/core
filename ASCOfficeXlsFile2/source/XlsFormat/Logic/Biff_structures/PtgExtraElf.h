#pragma once

#include "Ptg.h"
#include "CellRef.h"

namespace XLS
{;

class CFRecord;

class PtgExtraElf : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgExtraElf)
public:
	PtgExtraElf();
	PtgExtraElf(CFRecord& record);
	BiffStructurePtr clone();

	
	virtual void load(CFRecord& record); // No type info
	virtual void store(CFRecord& record); // No type info

	const std::wstring toString() const;
	const std::wstring toColumnsSet() const;
private:
	bool fRel;
	std::vector<RgceElfLocExtra> array_;
};

typedef boost::shared_ptr<PtgExtraElf> PtgExtraElfPtr;

} // namespace XLS

