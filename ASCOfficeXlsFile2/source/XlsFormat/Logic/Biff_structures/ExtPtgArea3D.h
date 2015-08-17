#pragma once

#include "Ptg.h"
#include "CellRangeRef.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{

class CFRecord;

class ExtPtgArea3D : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ExtPtgArea3D)
public:
	ExtPtgArea3D(const CellRef& cell_base_ref_init);
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

	ExtSheetPair iTabs;
	RgceAreaRel area;
	const CellRef& cell_base_ref;
};

} // namespace XLS

