#pragma once

#include "OperandPtg.h"

namespace XLS
{;

class CFRecord;

class PtgAreaErr: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgAreaErr)
public:
	PtgAreaErr();
	PtgAreaErr(const PtgDataType data_type_init);
	BiffStructurePtr clone();

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	virtual const unsigned short getPtgId() const	{ return id_fixed | (static_cast<unsigned char>(data_type) << 5);}

private:
	static const unsigned short id_fixed = 0x0A;
	unsigned char data_type;
};

} // namespace XLS

