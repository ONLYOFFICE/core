#pragma once

#include "OperandPtg.h"
#include "CellRangeRef.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class PtgArea: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgArea)
public:
	PtgArea();
	PtgArea(const std::wstring& word, const PtgDataType data_type);
	BiffStructurePtr clone();

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

	static const unsigned short fixed_id = 0x05;

	RgceArea area;
};

} // namespace XLS

