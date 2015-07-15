#pragma once

#include "OperandPtg.h"
#include "Boolean.h"

namespace XLS
{;

class CFRecord;

class PtgBool: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgBool)
public:
	PtgBool();
	PtgBool(const std::wstring& word);
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned __int16 fixed_id = 0x1D;

private:
	Boolean<unsigned char> boolean_;
};

} // namespace XLS

