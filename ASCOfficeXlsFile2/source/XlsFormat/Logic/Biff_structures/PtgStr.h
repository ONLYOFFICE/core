#pragma once

#include "OperandPtg.h"
#include "BiffString.h"

namespace XLS
{;

class CFRecord;

class PtgStr: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgStr)
public:
	PtgStr();
	PtgStr(const std::wstring & str);
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned __int16 fixed_id = 0x17;

private:
	ShortXLUnicodeString string_;
};

} // namespace XLS

