#pragma once

#include "OperandPtg.h"

namespace XLS
{;

class CFRecord;

class PtgElfRwV: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgElfRwV)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);

private:
	RgceElfLoc loc;
};

} // namespace XLS

