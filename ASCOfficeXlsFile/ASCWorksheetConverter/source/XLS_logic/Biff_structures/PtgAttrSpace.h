#pragma once

#include "Ptg.h"
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class PtgAttrSpace : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgAttrSpace)
public:
	BiffStructurePtr clone();

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);

};

} // namespace XLS

