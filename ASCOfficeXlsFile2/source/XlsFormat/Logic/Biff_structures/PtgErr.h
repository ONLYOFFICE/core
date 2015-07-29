#pragma once

#include "OperandPtg.h"
#include "BitMarkedStructs.h"

namespace XLS
{;

class CFRecord;

class PtgErr: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgErr)
public:
	PtgErr();
	PtgErr(const std::wstring  str);
	BiffStructurePtr clone();

	
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

	static const unsigned short fixed_id = 0x1C;

private:
	BErr err;
};

} // namespace XLS

