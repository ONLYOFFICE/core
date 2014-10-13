#pragma once

#include "OperandPtg.h"
#include "BitMarkedStructs.h"

namespace XLS
{;

class CFRecord;

class PtgRef: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgRef)
public:
	PtgRef();
	PtgRef(const std::wstring& word, const PtgDataType data_type);
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned __int16 fixed_id = 0x04;
private:
	RgceLoc loc;
};

} // namespace XLS

