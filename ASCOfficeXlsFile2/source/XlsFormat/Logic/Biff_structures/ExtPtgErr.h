#pragma once

#include "Ptg.h"
#include "BitMarkedStructs.h"

namespace XLS
{;

class CFRecord;

class ExtPtgErr : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ExtPtgErr)
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

private:
	BErr err;
};

} // namespace XLS

