#pragma once

#include "OperandPtg.h"

namespace XLS
{;

class PtgMissArg: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgMissArg)
public:
	PtgMissArg();
	BiffStructurePtr clone();

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);
	static const WORD fixed_id = 0x16;
};

} // namespace XLS

