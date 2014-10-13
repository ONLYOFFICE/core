#pragma once

#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class PtgMemNoMemN : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgMemNoMemN)
public:
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

private:
	unsigned char type;
	unsigned __int16 cce;
};

} // namespace XLS

