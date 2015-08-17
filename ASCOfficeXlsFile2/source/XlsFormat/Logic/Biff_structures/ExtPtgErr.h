#pragma once

#include "Ptg.h"
#include "BitMarkedStructs.h"

namespace XLS
{

class CFRecord;

class ExtPtgErr : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ExtPtgErr)
public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

	BErr err;
};

} // namespace XLS

