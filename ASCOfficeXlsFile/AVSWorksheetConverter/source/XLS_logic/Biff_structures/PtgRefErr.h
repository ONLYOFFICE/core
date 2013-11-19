#pragma once

#include "OperandPtg.h"

namespace XLS
{;

class CFRecord;

class PtgRefErr: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgRefErr)
public:
	PtgRefErr();
	PtgRefErr(const PtgDataType data_type_init);
	BiffStructurePtr clone();

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);

	static const WORD fixed_id = 0x0A;
};

} // namespace XLS

