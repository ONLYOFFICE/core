#pragma once

#include "OperandPtg.h"

namespace XLS
{;

class CFRecord;

class PtgNum: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgNum)
public:
	PtgNum();
	PtgNum(const std::wstring& word);
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned __int16 fixed_id = 0x1F;

private:
	double value_;
};

} // namespace XLS

