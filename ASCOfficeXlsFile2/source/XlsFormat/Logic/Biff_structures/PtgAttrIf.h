#pragma once

#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class PtgAttrIf : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgAttrIf)
public:
	BiffStructurePtr clone();

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

	unsigned short offset;
};

} // namespace XLS

