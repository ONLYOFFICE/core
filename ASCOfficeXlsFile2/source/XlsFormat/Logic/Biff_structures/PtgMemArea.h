#pragma once

#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class PtgMemArea : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgMemArea)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

private:
	unsigned short cce;
};

} // namespace XLS

