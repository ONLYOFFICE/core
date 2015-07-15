#pragma once

#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class PtgAttrSum : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgAttrSum)
public:
	BiffStructurePtr clone();

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned __int16 fixed_id = 0x1019;
};

} // namespace XLS

