#pragma once

#include "Ptg.h"
#include "Cetab.h"
#include "Ftab_Cetab.h"

namespace XLS
{;

class CFRecord;

class PtgFuncVar : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgFuncVar)
	PtgFuncVar(const unsigned short func_index, const unsigned char data_type);
public:
	PtgFuncVar();
	BiffStructurePtr clone();

	static PtgPtr create(const std::wstring& word, const unsigned char data_type);

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	void setParamsNum(const unsigned char num);

	static const unsigned short fixed_id = 0x02;
private:
	unsigned char cparams;
	Ftab_Cetab tab;
	bool fCeFunc;
};

typedef boost::shared_ptr<PtgFuncVar> PtgFuncVarPtr;


} // namespace XLS

