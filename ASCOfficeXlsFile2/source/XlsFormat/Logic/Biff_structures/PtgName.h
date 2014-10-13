#pragma once

#include "OperandPtg.h"
#include "BitMarkedStructs.h"

namespace XLS
{;

class CFRecord;

class PtgName: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgName)
public:
	PtgName();
	PtgName(const unsigned int index, const PtgDataType data_type_init);
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned __int16 fixed_id = 0x03;

private:
	unsigned int nameindex;
};

} // namespace XLS

