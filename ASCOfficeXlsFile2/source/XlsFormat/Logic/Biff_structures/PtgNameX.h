#pragma once

#include "OperandPtg.h"
#include "Logic/GlobalWorkbookInfo.h"

namespace XLS
{

class CFRecord;

class PtgNameX: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgNameX)
public:
	PtgNameX();
	PtgNameX(const std::wstring& word, const PtgDataType data_type_init);
	BiffStructurePtr clone();

	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

	static const unsigned short fixed_id = 0x19;

	void fromString(const std::wstring& word);

	unsigned short ixti;
	unsigned int nameindex;
private:
	GlobalWorkbookInfoPtr global_info;
};

typedef boost::shared_ptr<PtgNameX> PtgNameXPtr;

} // namespace XLS

