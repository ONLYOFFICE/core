#pragma once

#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class PtgAttrChoose : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgAttrChoose)
public:
	BiffStructurePtr clone();

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

	unsigned short cOffset;
	std::vector<unsigned short> rgOffset;
};

} // namespace XLS

