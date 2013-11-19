#pragma once

#include "OperandPtg.h"

namespace XLS
{;

class CFRecord;

class PtgInt: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgInt)
public:
	PtgInt();
	PtgInt(const std::wstring& word);
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);

	static const WORD fixed_id = 0x1E;

private:
	WORD integer;
};

} // namespace XLS

