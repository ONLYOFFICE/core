#pragma once

#include "OperandPtg.h"
#include "BitMarkedStructs.h"

namespace XLS
{;

class CFRecord;

class PtgElfRadicalLel: public OperandPtg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgElfRadicalLel)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

private:
	Ilel ilel;
	bool fQuoted;
};

} // namespace XLS

