#pragma once

#include "OperandPtg.h"

namespace XLS
{;

class CFRecord;

class PtgArray: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgArray)
public:
	PtgArray();
	PtgArray(const PtgDataType data_type);
	BiffStructurePtr clone();

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned short fixed_id = 0x00;
};

} // namespace XLS

