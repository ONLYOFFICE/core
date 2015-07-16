#pragma once

#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class PtgExp : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgExp)
public:
	PtgExp();
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned short fixed_id = 0x01;

private:
	unsigned short row;
	unsigned short col;
};

typedef boost::shared_ptr<PtgExp> PtgExpPtr;

} // namespace XLS

