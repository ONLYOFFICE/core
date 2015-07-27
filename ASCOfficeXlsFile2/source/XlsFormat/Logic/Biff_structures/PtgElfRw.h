#pragma once

#include "OperandPtg.h"

namespace XLS
{;

class CFRecord;

class PtgElfRw: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgElfRw)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

private:
	RgceElfLoc loc;
};

} // namespace XLS

