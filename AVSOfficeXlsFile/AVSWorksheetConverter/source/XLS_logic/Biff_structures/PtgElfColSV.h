#pragma once

#include "OperandPtg.h"

namespace XLS
{;

class CFRecord;

class PtgElfColSV: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgElfColSV)
public:
	BiffStructurePtr clone();

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);

};

} // namespace XLS

