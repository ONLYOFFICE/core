#pragma once

#include "OperandPtg.h"

namespace XLS
{;

class CFRecord;

class PtgRefErr: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgRefErr)
public:
	PtgRefErr();
	PtgRefErr(const PtgDataType data_type_init);
	BiffStructurePtr clone();

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned __int16 fixed_id = 0x0A;
};

} // namespace XLS

