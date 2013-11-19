#pragma once

#include "OperandPtg.h"
#include "XLS_logic/GlobalWorkbookInfo.h"

namespace XLS
{;

class CFRecord;

class PtgRefErr3d: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgRefErr3d)
public:
	PtgRefErr3d();
	PtgRefErr3d(const WORD ixti, const PtgDataType data_type);
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);

	static const WORD fixed_id = 0x1C;
private:
	WORD ixti;
	GlobalWorkbookInfoPtr global_info;
};

} // namespace XLS

