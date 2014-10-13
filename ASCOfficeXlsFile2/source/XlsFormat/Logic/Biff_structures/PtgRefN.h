#pragma once

#include "OperandPtg.h"

namespace XLS
{;

class CFRecord;

class PtgRefN: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgRefN)
public:
	PtgRefN(const CellRef& cell_base_ref_init);
	PtgRefN(const std::wstring& word, const PtgDataType data_type, const CellRef& cell_base_ref);
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned __int16 fixed_id = 0x0C;

private:
	CellRef cell_base_ref;
	RgceLocRel loc;
};

} // namespace XLS

