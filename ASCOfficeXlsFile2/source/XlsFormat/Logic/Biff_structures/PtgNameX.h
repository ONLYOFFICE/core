#pragma once

#include "OperandPtg.h"
#include "Logic/GlobalWorkbookInfo.h"

namespace XLS
{;

class CFRecord;

class PtgNameX: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgNameX)
public:
	PtgNameX();
	PtgNameX(const std::wstring& word, BiffStructurePtr & parent, const PtgDataType data_type_init);
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned short fixed_id = 0x19;
private:
	void fromString(const std::wstring& word, BiffStructurePtr & parent);

private:
	unsigned short ixti;
	unsigned int nameindex;
	GlobalWorkbookInfoPtr global_info;
};

typedef boost::shared_ptr<PtgNameX> PtgNameXPtr;

} // namespace XLS

