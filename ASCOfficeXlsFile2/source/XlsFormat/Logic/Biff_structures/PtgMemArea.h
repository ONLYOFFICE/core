#pragma once

#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class PtgMemArea : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgMemArea)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

private:
	unsigned __int16 cce;
};

} // namespace XLS

