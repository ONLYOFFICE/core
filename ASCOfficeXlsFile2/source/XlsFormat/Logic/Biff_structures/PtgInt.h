#pragma once

#include "OperandPtg.h"

namespace XLS
{

class CFRecord;

class PtgInt: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgInt)
public:
	PtgInt();
	PtgInt(const std::wstring& word);
	BiffStructurePtr clone();

	
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

	static const unsigned short fixed_id = 0x1E;

private:
	unsigned short integer;
};

} // namespace XLS

