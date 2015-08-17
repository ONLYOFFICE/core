#pragma once

#include "OperandPtg.h"

namespace XLS
{

class PtgMissArg: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgMissArg)
public:
	PtgMissArg();
	BiffStructurePtr clone();

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);
	static const unsigned short fixed_id = 0x16;
};

} // namespace XLS

