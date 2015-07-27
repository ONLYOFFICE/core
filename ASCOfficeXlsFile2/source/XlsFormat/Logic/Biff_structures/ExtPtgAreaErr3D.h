#pragma once

#include "Ptg.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class ExtPtgAreaErr3D : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ExtPtgAreaErr3D)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

	ExtSheetPair iTabs;
};

} // namespace XLS

