#pragma once

#include "OperandPtg.h"
#include "BitMarkedStructs.h"

namespace XLS
{

class CFRecord;

class PtgName: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgName)
public:
	PtgName();
	PtgName(const unsigned int index, const PtgDataType data_type_init);
	BiffStructurePtr clone();

	
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref = false);

	static const unsigned short fixed_id = 0x03;

	_UINT16 nameindex;
private:
	GlobalWorkbookInfoPtr global_info;
};

} // namespace XLS

