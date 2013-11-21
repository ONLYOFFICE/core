#pragma once

#include "OperandPtg.h"
#include "BiffString.h"

namespace XLS
{;

class CFRecord;

class PtgStr: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgStr)
public:
	PtgStr();
	PtgStr(const _bstr_t& str);
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);

	static const WORD fixed_id = 0x17;

private:
	ShortXLUnicodeString string_;
};

} // namespace XLS

