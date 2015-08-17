#pragma once

#include "OperandPtg.h"
#include "BitMarkedStructs.h"

namespace XLS
{

class CFRecord;

class PtgElfLel: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgElfLel)
public:
	BiffStructurePtr clone();

	
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

private:
	Ilel ilel;
	bool fQuoted;
};

} // namespace XLS

