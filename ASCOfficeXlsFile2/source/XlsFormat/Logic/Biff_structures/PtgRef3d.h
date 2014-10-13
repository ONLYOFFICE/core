#pragma once

#include "OperandPtg.h"
#include "Logic/GlobalWorkbookInfo.h"

namespace XLS
{;

class CFRecord;

class PtgRef3d: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgRef3d)
public:
	PtgRef3d(const CellRef& cell_base_ref_init);
	PtgRef3d(const unsigned __int16 ixti, const std::wstring& word, const PtgDataType data_type, const CellRef& cell_base_ref_init);
	BiffStructurePtr clone();

	void fromString(const std::wstring& str);

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned __int16 fixed_id = 0x1A;
private:
	unsigned __int16 ixti;
	RgceLocRel rgce_loc_rel;
	RgceLoc rgce_loc;
	const CellRef& cell_base_ref;
	GlobalWorkbookInfoPtr global_info;

};

} // namespace XLS

