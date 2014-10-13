#pragma once

#include "Ptg.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class ExtPtgRef3D : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ExtPtgRef3D)
public:
	ExtPtgRef3D(const CellRef& cell_base_ref_init);
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

private:
	ExtSheetPair iTabs;
	RgceLocRel loc;
	const CellRef& cell_base_ref;
};

} // namespace XLS

