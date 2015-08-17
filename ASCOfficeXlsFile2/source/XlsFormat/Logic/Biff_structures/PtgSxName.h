#pragma once

#include "OperandPtg.h"

namespace XLS
{

class CFRecord;

class PtgSxName: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgSxName)
public:
	BiffStructurePtr clone();

	
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

private:
	unsigned int sxIndex;
};

} // namespace XLS

