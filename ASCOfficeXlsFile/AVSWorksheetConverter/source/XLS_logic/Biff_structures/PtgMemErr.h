#pragma once

#include "Ptg.h"
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class PtgMemErr : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgMemErr)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);

private:
	BErr err;
	WORD cce;
};

} // namespace XLS

