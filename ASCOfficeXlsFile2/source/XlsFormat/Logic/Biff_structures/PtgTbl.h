#pragma once

#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class PtgTbl : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgTbl)
public:
	PtgTbl();
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	static const unsigned __int16 fixed_id = 0x02;

private:
	unsigned __int16 row;
	unsigned __int16 col;
};

typedef boost::shared_ptr<PtgTbl> PtgTblPtr;

} // namespace XLS

