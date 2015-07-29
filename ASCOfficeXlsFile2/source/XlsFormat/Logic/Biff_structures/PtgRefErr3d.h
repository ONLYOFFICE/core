#pragma once

#include "OperandPtg.h"
#include "Logic/GlobalWorkbookInfo.h"

namespace XLS
{;

class CFRecord;

class PtgRefErr3d: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgRefErr3d)
public:
	PtgRefErr3d();
	PtgRefErr3d(const unsigned short ixti, const PtgDataType data_type);
	BiffStructurePtr clone();

	
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

	static const unsigned short fixed_id = 0x1C;
private:
	unsigned short ixti;
	GlobalWorkbookInfoPtr global_info;
};

} // namespace XLS

