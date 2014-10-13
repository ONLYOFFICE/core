#pragma once

#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class PtgAttrChoose : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgAttrChoose)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

private:
	unsigned __int16 cOffset;
	std::vector<unsigned __int16> rgOffset;
};

} // namespace XLS

