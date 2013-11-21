#pragma once

#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class PtgTbl : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgTbl)
public:
	PtgTbl();
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);

	static const WORD fixed_id = 0x02;

private:
	WORD row;
	WORD col;
};

typedef boost::shared_ptr<PtgTbl> PtgTblPtr;

} // namespace XLS

